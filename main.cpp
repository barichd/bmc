// Copyright (C) 2011  Mario Lang <mlang@delysid.org>
//
// Distributed under the terms of the GNU General Public License version 3.
// (see accompanying file LICENSE.txt or copy at
//  http://www.gnu.org/licenses/gpl-3.0-standalone.html)

#include "ttb/ttb.h"

#include "config.hpp"
#include "ambiguous.hpp"
#include "compiler.hpp"
#include "fluidsynth.hpp"
#include "lilypond.hpp"
#include "score.hpp"

#include <boost/spirit/include/qi_parse.hpp>
#include <thread>

int
main(int argc, char const *argv[])
{
  std::locale::global(std::locale(""));

  {
    char *localeTable = selectTextTable(TABLES_DIRECTORY);
    if (localeTable) {
      replaceTextTable(TABLES_DIRECTORY, localeTable);
      free(localeTable);
    }
  }

  std::istreambuf_iterator<wchar_t> wcin_begin(std::wcin.rdbuf()), wcin_end;
  std::wstring source(wcin_begin, wcin_end);
  typedef std::wstring::const_iterator iterator_type;
  iterator_type iter = source.begin();
  iterator_type const end = source.end();
  typedef music::braille::error_handler<iterator_type> error_handler_type;
  error_handler_type error_handler(iter, end);
  typedef music::braille::score_grammar<iterator_type> parser_type;
  parser_type parser(error_handler);
  boost::spirit::traits::attribute_of<parser_type>::type score;

  bool const success = parse(iter, end, parser, score);

  if (success && iter == end) {
    music::braille::compiler<error_handler_type> compile(error_handler);
    if (compile(score)) {
      std::thread player;

      if (argc == 2 and strcmp(argv[1], "-p") == 0)
        player = std::thread(music::fluidsynth(SOUNDFONT_PATH), score);

      music::lilypond_output_format(std::cout);
      std::cout << score;

      if (player.joinable()) player.join();

      return EXIT_SUCCESS;
    }
  }

  return EXIT_FAILURE;
}


