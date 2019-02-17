parse_node parse_enum_field(lexer& l) {
  std::vector<parse_node> children;
  children.push_back(parse_identifier(l));
  if (l.peek() && l.peek()->val == "(") {
    children.push_back({ *l.next(), {} });
    children.push_back(parse_expression(l));
    PARSE_ASSERT_VAL(l, ")");
    children.push_back({ *l.next(), {} });
  }
  return { nterm::enum_field, children };
}
