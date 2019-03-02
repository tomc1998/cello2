
parse_node parse_range(lexer& l, parse_node lrec) {
  std::vector<parse_node> children { lrec };
  PARSE_ASSERT_VAL(l, "..");
  children.push_back({ *l.next(), {}, l.get_curr_source_label() });
  children.push_back(parse_expression(l));
  return { nterm::range, children, l.get_curr_source_label() };
}
