parse_node parse_empty_array_access(lexer& l, parse_node lrec) {
  std::vector<parse_node> children { lrec };
  PARSE_ASSERT_VAL(l, "[");
  children.push_back({ *l.next(), {}, l.get_curr_source_label() });
  PARSE_ASSERT_VAL(l, "]");
  children.push_back({ *l.next(), {}, l.get_curr_source_label() });
  return { nterm::empty_array_access, children, l.get_curr_source_label() };
}
