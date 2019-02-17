
parse_node parse_literal(lexer& l) {
  PARSE_ASSERT_NOT_EMPTY(l, "Expected literal, got EOF");
  if (l.peek()->type != token_type::int_lit &&
      l.peek()->type != token_type::float_lit &&
      l.peek()->type != token_type::string_lit &&
      l.peek()->type != token_type::c_string_lit) {
    throw new parse_error(l, std::string("Expected literal, got ")
                          + std::string(l.peek()->val));
  }
  return { nterm::literal, { { *l.next(), {} } } };
}
