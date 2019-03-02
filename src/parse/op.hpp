parse_node parse_op(lexer& l) {
  PARSE_ASSERT_TYPE(l, token_type::op, "operator");
  return { nterm::op, { { *l.next(), {}, l.get_curr_source_label() } }, l.get_curr_source_label() };
}
