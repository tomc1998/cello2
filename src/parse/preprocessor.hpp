parse_node parse_preprocessor(lexer& l) {
  std::vector<parse_node> children;
  PARSE_ASSERT_VAL(l, "#");
  children.push_back({ *l.next(), {} });

  PARSE_ASSERT_NOT_EMPTY(l, "Expected preprocessor command, got EOF");
  if (l.peek()->val == "include") {
    children.push_back({*l.next(), {}});
    children.push_back(parse_identifier(l));
  } else if (l.peek()->val == "import") {
    children.push_back({*l.next(), {}});
    children.push_back(parse_identifier(l));
  } else {
    throw parse_error(l, std::string("Undefined preprocessor command '")
                      + std::string(l.peek()->val) + "'");
  }
  return { nterm::preprocessor, children };
}
