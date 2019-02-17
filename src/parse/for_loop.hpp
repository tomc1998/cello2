parse_node parse_for_loop(lexer& l) {
  std::vector<parse_node> children;
  PARSE_ASSERT_VAL(l, "for");
  children.push_back({ *l.next(), {} });
  children.push_back(parse_identifier(l));
  PARSE_ASSERT_VAL(l, "in");
  children.push_back({ *l.next(), {} });
  children.push_back(parse_expression(l));
  children.push_back(parse_expression(l));
  return { nterm::for_loop, children };
}
