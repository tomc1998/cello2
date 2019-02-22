parse_node parse_assignment(lexer& l, parse_node lrec) {
  std::vector<parse_node> children { lrec };
  PARSE_ASSERT_VAL(l, "=");
  children.push_back({ *l.next(), {}});
  children.push_back(parse_expression(l));
  return { nterm::assignment, children };
}
