
parse_node parse_identifier(lexer& l) {
  PARSE_ASSERT_TYPE(l, token_type::ident, "Identifier");
  return { nterm::identifier, { { *l.next(), {} } } };
}
