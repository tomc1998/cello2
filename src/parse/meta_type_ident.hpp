
parse_node parse_meta_type_ident(lexer& l) {
  PARSE_ASSERT_VAL(l, "$");
  PARSE_ASSERT_NOT_EMPTY(l, "Expected meta type (i.e. '$type', '$expr', etc.), found EOF");
  return { nterm::meta_type_ident, { { *l.next(), {} }, parse_identifier(l) } };
}
