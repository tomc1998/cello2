

parse_node parse_fn_type(lexer& l) {
  PARSE_ASSERT_VAL(l, "fn");
  return { nterm::fn_type, { { *l.next(), {} }, parse_fn_signature(l) } };
}
