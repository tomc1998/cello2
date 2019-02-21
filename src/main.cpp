#include <nonstd/optional.hpp>
#include <nonstd/string_view.hpp>
#include <mapbox/variant.hpp>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <cstdint>
#include <regex>

// Only including this to throw runtime_errors. Can be much better than asserts.
#include <exception>

// Put variant into nonstd namespace, 'cause it's f u c k i n g annoying to have
// to type out mapbox::util:: all the time
namespace nonstd {
  using namespace mapbox::util;
}

#include "arg_parse.hpp"
#include "source_label.hpp"
#include "lexer.hpp"
#include "parse_error.hpp"
#include "parser.hpp"

int main(int argc, const char** argv) {

  const auto args = parse_prog_arg_list(argc, argv);
  if (args.num_positional() == 0) {
    std::cerr << "Please pass a file to compile" << std::endl;
    return 1;
  } else if (args.num_positional() > 1) {
    std::cerr << "2 positional args passed - compiler only supports compiling 1 file" << std::endl;
    return 1;
  }
  auto file_name = std::string(args.get_positional(0).name);
  std::ifstream t(file_name);
  if (!t.good()) {
    std::cerr << "Can't read file '" << file_name << "'" << std::endl;
    return 1;
  }
  std::string file((std::istreambuf_iterator<char>(t)),
                   std::istreambuf_iterator<char>());

  lexer l(file_name, file);
  const auto tree = parse(l);

  return 0;
}
