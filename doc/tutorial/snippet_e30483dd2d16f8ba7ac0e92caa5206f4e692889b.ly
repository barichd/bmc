% Automatically generated by BMC, the braille music compiler
\version "2.14.2"
\include "articulate.ly"
\header {
  tagline = ""
}
music =
  <<
    \new Staff {
      \clef "treble"
      << {g'8 e' << {a'8. g'16 f'8 e'16 f' g'[ f' e' d']}\\{c'8 a d'8. c'16 b8 a16 b} >>}\\{g1} >> | % 1
    }
  >>

\score {
  \music
  \layout { }
}
\score {
  \unfoldRepeats \articulate \music
  \midi { }
}
