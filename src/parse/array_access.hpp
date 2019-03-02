
parse_node parse_array_access(lexer& l, parse_node lrec) {
  std::vector<parse_node> children { lrec };
  PARSE_ASSERT_VAL(l, "[");
  children.push_back({ *l.next(), {}, l.get_curr_source_label() });
  children.push_back(parse_expression(l));
  PARSE_ASSERT_VAL(l, "]");
  children.push_back({ *l.next(), {}, l.get_curr_source_label() });
  return { nterm::array_access, children, l.get_curr_source_label() };
}
