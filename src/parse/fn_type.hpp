

parse_node parse_fn_type(lexer& l) {
  PARSE_ASSERT_VAL(l, "fn");
  return { nterm::fn_type, { { *l.next(), {}, l.get_curr_source_label() }, parse_fn_signature(l) }, l.get_curr_source_label() };
}
