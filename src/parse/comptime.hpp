parse_node parse_comptime(lexer& l) {
  PARSE_ASSERT_VAL(l, "comptime");
  return { nterm::comptime, { { *l.next(), {} }, parse_statement_list(l) } };
}
