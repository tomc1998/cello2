
parse_node parse_var_declaration(lexer& l) {
  std::vector<parse_node> children;
  if (l.peek()->val == "export") {
    children.push_back({ *l.next(), {} });
  }
  if (l.peek()->val == "comptime") {
    children.push_back({ *l.next(), {} });
  }
  if (l.peek()->val != "var" && l.peek()->val != "mut") {
    throw parse_error(l, std::string("Expected 'var' or 'mut', got ")
                      + std::string(l.peek()->val));
  }
  children.push_back({ *l.next(), {} });
  children.push_back(parse_identifier(l));
  PARSE_ASSERT_NOT_EMPTY(l, "Expected ':' or '=', found EOF");
  if (l.peek()->val == ":") {
    children.push_back({ *l.next(), {} });
    children.push_back(parse_expression(l));
  };
  PARSE_ASSERT_VAL(l, "=");
  children.push_back({ *l.next(), {} });
  children.push_back(parse_expression(l));
  return { nterm::var_declaration, children };
}
