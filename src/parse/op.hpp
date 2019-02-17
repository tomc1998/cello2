parse_node parse_op(lexer& l) {
  PARSE_ASSERT_TYPE(l, token_type::op, "operator");
  return { nterm::op, { { *l.next(), {} } } };
}
