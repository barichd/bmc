// Copyright (C) 2011  Mario Lang <mlang@delysid.org>
//
// Distributed under the terms of the GNU General Public License version 3.
// (see accompanying file LICENSE.txt or copy at
//  http://www.gnu.org/licenses/gpl-3.0-standalone.html)

#if !defined(BMC_TIME_SIGNATURE_HPP)
#define BMC_TIME_SIGNATURE_HPP

#include "config.hpp"
#include "bmc/music.hpp"
#include "bmc/braille/parsing/grammar/numbers.hpp"
#include <boost/spirit/include/qi_grammar.hpp>

namespace bmc { namespace braille {

/**
 * \brief A grammar for parsing a braille time signature.
 *
 * \ingroup grammar
 */
template <typename Iterator>
struct time_signature_grammar
: boost::spirit::qi::grammar< Iterator
                            , ::bmc::time_signature()
                            , boost::spirit::qi::locals<unsigned>
                            >
{
  time_signature_grammar();

  boost::spirit::qi::rule< Iterator
                         , ::bmc::time_signature()
                         , boost::spirit::qi::locals<unsigned>
                         > start;
  upper_number_grammar<Iterator> upper_number;
  lower_number_grammar<Iterator> lower_number;
};

}}

#endif

