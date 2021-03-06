// Copyright (C) 2014  Mario Lang <mlang@delysid.org>
//
// Distributed under the terms of the GNU General Public License version 3.
// (see accompanying file LICENSE.txt or copy at
//  http://www.gnu.org/licenses/gpl-3.0-standalone.html)

#include "bmc/musicxml.hpp"
#include "xsdcxx-musicxml/musicxml.hpp"

namespace bmc {

namespace {

// We are going to export score-partwise documents.
using score_type = ::musicxml::score_partwise;
using part_type = score_type::part_type;
using measure_type = part_type::measure_type;

// Determine the greatest common divisor of all rhythmic values in a braille score.
// We need this for the MusicXML divisons element.
// We're basically determining the common denominator such that all
// rhythmic values can be expressed as an integer, since the MusicXML duration
// element is not a rational.

class duration_gcd_visitor : public boost::static_visitor<void> {
  rational value = 0;
public:
  void operator()(braille::ast::unfolded::measure const &measure) {
    for (auto &&voice: measure.voices) {
      for (auto &&partial_measure: voice) {
        for (auto &&partial_voice: partial_measure) {
          std::for_each(partial_voice.begin(), partial_voice.end(), apply_visitor(*this));
        }
      }
    }
  }
  void operator()(::bmc::braille::ast::rhythmic const &r) {
    value = boost::math::gcd(value, r.as_rational());
  }
  void operator()(braille::ast::clef const &) {}
  void operator()(braille::ast::key_and_time_signature const &) {}
  void operator()(braille::ast::tie const &) {}
  void operator()(braille::ast::tuplet_start const &) {}

  rational get() const { return value; }
};

rational duration_gcd(braille::ast::score const &score) {
  duration_gcd_visitor accumulator { };

  for (auto &&part: score.unfolded_part) {
    for (auto &&staff: part) {
      std::for_each(staff.begin(), staff.end(), apply_visitor(accumulator));
    }
  }

  return accumulator.get();
}
 
// Conversion functions between braille AST and MusicXML objects:

::musicxml::key xml(key_signature const &key) {
  ::musicxml::key xml_key{};
  xml_key.fifths(key);

  return xml_key;
}

::musicxml::time xml(time_signature const &time) {
  ::musicxml::time xml_time{};
  xml_time.beats().push_back(std::to_string(time.numerator()));
  xml_time.beat_type().push_back(std::to_string(time.denominator()));

  return xml_time;
}

::musicxml::positive_divisions duration(rational const &dur, rational const &divisions) {
  return {
    boost::rational_cast<double>(dur / (rational{1, 4} / divisions))
  };
}

::musicxml::backup backup(rational const &dur, rational const &divisions) {
  return { duration(dur, divisions) };
}

std::string to_string(rational const & r) {
  return std::to_string(r.numerator()) + "/" + std::to_string(r.denominator());
}

::musicxml::note_type note_type(rational const &r) {
  BOOST_ASSERT(r.numerator() == 1);
  switch (r.denominator()) {
  case 1: return ::musicxml::note_type_value::whole;
  case 2: return ::musicxml::note_type_value::half;
  case 4: return ::musicxml::note_type_value::quarter;
  case 8: return ::musicxml::note_type_value::eighth;
  case 16: return ::musicxml::note_type_value::cxx_16th;
  case 32: return ::musicxml::note_type_value::cxx_32nd;
  case 64: return ::musicxml::note_type_value::cxx_64th;
  case 128: return ::musicxml::note_type_value::cxx_128th;
  default: throw std::runtime_error("Unknown note type: " + to_string(r));
  }
}

::musicxml::pitch pitch(braille::ast::pitched const &p) {
  ::musicxml::step::value step;
  switch (p.step) {
  case A: step = ::musicxml::step::A; break;
  case B: step = ::musicxml::step::B; break;
  case C: step = ::musicxml::step::C; break;
  case D: step = ::musicxml::step::D; break;
  case E: step = ::musicxml::step::E; break;
  case F: step = ::musicxml::step::F; break;
  case G: step = ::musicxml::step::G; break;
  default: BOOST_ASSERT(false);
  }
  ::musicxml::pitch xml_pitch { step, p.octave - 1 };

  if (p.alter) xml_pitch.alter(p.alter);

  return xml_pitch;
}

::musicxml::accidental accidental(::bmc::accidental const &a) {
  switch (a) {
  case ::bmc::natural: return { ::musicxml::accidental_value::natural };
  case ::bmc::flat:    return { ::musicxml::accidental_value::flat };
  case ::bmc::sharp:   return { ::musicxml::accidental_value::sharp };
  case ::bmc::double_flat: return { ::musicxml::accidental_value::flat_flat };
  case ::bmc::double_sharp: return { ::musicxml::accidental_value::sharp_sharp };
  default: throw std::runtime_error("Invalid accidental: " + std::to_string(a));
  }
}

class fingering_visitor : public boost::static_visitor<void> {
  ::musicxml::technical::fingering_sequence &xml_fingers;
public:
  fingering_visitor(::musicxml::technical::fingering_sequence &xml_fingers)
  : xml_fingers { xml_fingers }
  {}

  void operator()(unsigned f) {
    xml_fingers.push_back({ std::to_string(f) });
  }
  void operator()(braille::finger_change const &fc) {
  }
};

::musicxml::technical::fingering_sequence
fingering(braille::fingering_list const &fingers) {
  ::musicxml::technical::fingering_sequence xml_fingers;

  BOOST_ASSERT(fingers.size() == 1);

  if (not fingers.empty()) {
    fingering_visitor visitor { xml_fingers };
    apply_visitor(visitor, fingers.front());
  }

  return xml_fingers;
}

class make_measures_for_staff_visitor : public boost::static_visitor<void> {
  braille::ast::score const &brl_score;
  part_type::measure_sequence &measures;
  unsigned staff_number;
  rational divisions;
  unsigned measure_number = 1;
  measure_type *current_measure;
public:
  make_measures_for_staff_visitor(braille::ast::score const &brl_score,
                                  part_type::measure_sequence &measures,
                                  unsigned staff_number,
                                  rational const &divisions)
  : brl_score { brl_score }
  , measures { measures }, staff_number { staff_number }, divisions { divisions }
  , current_measure { nullptr }
  {}

  void operator()(braille::ast::unfolded::measure const &measure) {
    if (measure_number > measures.size())
      measures.push_back({std::to_string(measure_number)});

    current_measure = &measures[measure_number - 1];

    if (measure_number == 1) {
      if ((not brl_score.time_sigs.empty() and (duration(measure) != brl_score.time_sigs.front())) or
          (brl_score.time_sigs.empty() and duration(measure) != 1)) {
        current_measure->implicit(::musicxml::yes_no::yes);
      }
    }
    if (staff_number > 1)
      current_measure->music_data().push_back(backup(duration(measure), divisions));
    for (auto vi = measure.voices.begin(), ve = measure.voices.end();
         vi != ve; ++vi) {
      for (auto &&partial_measure: *vi) {
        for (auto pvi = partial_measure.begin(), pve = partial_measure.end();
             pvi != pve; ++pvi) {
          std::for_each(pvi->begin(), pvi->end(), apply_visitor(*this));
          if (std::next(pvi) != pve)
            current_measure->music_data().push_back(backup(duration(*pvi), divisions));
        }
      }
      if (std::next(vi) != ve)
        current_measure->music_data().push_back(backup(duration(*vi), divisions));
    }

    measure_number += 1;
  }
  void operator()(braille::ast::key_and_time_signature const &) {
  }
  void operator()(braille::ast::note const &note) const {
    ::musicxml::note xml_note {
      pitch(note), duration(note.as_rational(), divisions)
    };

    if (is_grace(note)) {
      xml_note.grace(::musicxml::grace{});
    }
    xml_note.type(note_type(note.get_type()));
    for (unsigned dots = 0; dots < note.get_dots(); ++dots)
      xml_note.dot().push_back(::musicxml::empty_placement{});
    if (note.acc) xml_note.accidental(accidental(*note.acc));
    xml_note.staff(staff_number);

    auto xml_fingers = fingering(note.fingers);
    if (not xml_fingers.empty()) {
      ::musicxml::technical xml_technical { };
      xml_technical.fingering(xml_fingers);
      ::musicxml::notations xml_notations { };
      xml_notations.technical().push_back(xml_technical);      
      xml_note.notations().push_back(xml_notations);
    }
    current_measure->music_data().push_back(xml_note);
  }
  void operator()(braille::ast::rest const &rest) const {
    ::musicxml::note xml_note {
      ::musicxml::rest{}, duration(rest.as_rational(), divisions)
    };
    xml_note.type(note_type(rest.get_type()));
    for (unsigned dots = 0; dots < rest.get_dots(); ++dots)
      xml_note.dot().push_back(::musicxml::empty_placement{});
    xml_note.staff(staff_number);

    current_measure->music_data().push_back(xml_note);
  }
  void operator()(braille::ast::chord const &chord) const {
    (*this)(chord.base);
    for (auto &&interval: chord.intervals) {
      ::musicxml::note xml_note {
        pitch(interval), duration(chord.base.as_rational(), divisions)
      };

      xml_note.chord(::musicxml::empty{});
      xml_note.type(note_type(chord.base.get_type()));
      for (unsigned dots = 0; dots < chord.base.get_dots(); ++dots)
        xml_note.dot().push_back(::musicxml::empty_placement{});
      if (interval.acc) xml_note.accidental(accidental(*interval.acc));
      xml_note.staff(staff_number);

      current_measure->music_data().push_back(xml_note);
    }
  }
  void operator()(braille::ast::moving_note const &moving_note) const {
    (*this)(moving_note.base);
    current_measure->music_data().push_back(backup(moving_note.base.as_rational(), divisions));
    for (auto &&interval: moving_note.intervals) {
      ::musicxml::note xml_note {
        pitch(interval),
        duration(moving_note.base.as_rational() / moving_note.intervals.size(),
                 divisions)
      };

      xml_note.type(note_type(moving_note.base.get_type() / moving_note.intervals.size()));
      for (unsigned dots = 0; dots < moving_note.base.get_dots(); ++dots)
        xml_note.dot().push_back(::musicxml::empty_placement{});
      if (interval.acc) xml_note.accidental(accidental(*interval.acc));
      xml_note.staff(staff_number);

      current_measure->music_data().push_back(xml_note);
    }
  }

  void operator()(braille::ast::barline const &) const {
  }
  void operator()(braille::ast::clef const &) const {
  }
  void operator()(braille::hand_sign const &) const {
  }
  void operator()(braille::ast::tie const &) const {
  }
};

class musicxml_generator {
  braille::ast::score const &brl_score;
  score_type xml_score;
  ::musicxml::attributes global_attributes;
  rational divisions;

public:
  musicxml_generator(braille::ast::score const &score)
  : brl_score { score }, xml_score { ::musicxml::part_list {} }
  , global_attributes { }
    // The divisons element expresses the number of "ticks" per quarter note.
  , divisions {
      rational{1, 4} / boost::math::gcd(duration_gcd(brl_score), rational{1, 4})
    }
  {
    BOOST_ASSERT(divisions.denominator() == 1);

    xml_score.version("3.0");

    global_attributes.divisions(boost::rational_cast<double>(divisions));
    global_attributes.key().push_back(xml(brl_score.key_sig));
    if (not brl_score.time_sigs.empty())
      global_attributes.time().push_back(xml(brl_score.time_sigs.front()));

    unsigned c { 1 };
    for (auto &&p: brl_score.unfolded_part) {
      part_type part { "P" + std::to_string(c) };

      part.measure(get_measures(p));

      xml_score.part_list().score_part().push_back({
        "Part-" + std::to_string(c++), part.id()
      });
      xml_score.part().push_back(part);
    }
  }

  part_type::measure_sequence get_measures(braille::ast::unfolded::part const &p)
  {
    measure_type initial_measure { "1" };
    ::musicxml::attributes attributes { global_attributes };
    attributes.staves(p.size());
    initial_measure.music_data().push_back(attributes);

    part_type::measure_sequence measures;
    measures.push_back(initial_measure);

    unsigned staff_number { 1 };
    for (auto &&staff: p) {
      make_measures_for_staff_visitor visitor {
        brl_score, measures, staff_number, divisions
      };
      std::for_each(staff.begin(), staff.end(), apply_visitor(visitor));

      staff_number += 1;
    }

    return measures;
  }

  score_type score_partwise() const { return xml_score; }
};

}

void musicxml(std::ostream &os, ::bmc::braille::ast::score const &score)
{
  ::musicxml::serialize(os, musicxml_generator(score).score_partwise());
}

}

