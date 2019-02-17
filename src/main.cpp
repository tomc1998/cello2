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

#include "arg_parse.hpp"
#include "source_label.hpp"
#include "lexer.hpp"

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

  std::cout << std::endl;
  nonstd::optional<token> tok;
  while ((tok = l.next()).has_value()) {
    std::cout << "\"" << tok->val << "\"" << "(" << (int)tok->type << "), ";
  }
  std::cout << std::endl;

  return 0;
}
