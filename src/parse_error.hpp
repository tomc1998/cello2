
/** Thrown by the parsers. */
struct parse_error {
  source_label sl;
  std::string error;
  std::vector<parse_error> children;

  parse_error(lexer& l, std::string error)
    : sl(l.get_curr_source_label()), error(error) {};

  parse_error(source_label sl, std::string error)
    : sl(sl), error(error) {};
  parse_error() : sl(), error() {}

  void push_back(parse_error child) {
    children.push_back(child);
  }
};
