
parse_node parse_comptime_fn_declaration(lexer& l) {
  std::vector<parse_node> children;
  PARSE_ASSERT_VAL(l, "comptime");
  children.push_back({ *l.next(), {} });
  PARSE_ASSERT_VAL(l, "fn");
  children.push_back({ *l.next(), {} });
  children.push_back(parse_identifier(l));
  PARSE_ASSERT_NOT_EMPTY(l, "Expected rest of function declaration, got EOF");
  if (l.peek()->val == "(") {
    children.push_back(parse_parameter_decl_list(l));
  }
  PARSE_ASSERT_VAL(l, "=");
  children.push_back({ *l.next(), {} });
  children.push_back(parse_fn_signature(l));
  children.push_back(parse_expression(l));
  return { nterm::comptime_fn_declaration, children };
}
