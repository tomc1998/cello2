parse_node parse_comptime(lexer& l) {
  PARSE_ASSERT_VAL(l, "comptime");
  return { nterm::comptime, { { *l.next(), {}, l.get_curr_source_label() }, parse_statement_list(l) }, l.get_curr_source_label() };
}
