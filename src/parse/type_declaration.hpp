
parse_node parse_type_declaration(lexer& l) {
  PARSE_ASSERT_VAL(l, "type");
  std::vector<parse_node> children;
  children.push_back({ *l.next(), {} });
  children.push_back(parse_identifier(l));
  PARSE_ASSERT_NOT_EMPTY(l, "Expected rest of function declaration, got EOF");
  if (l.peek()->val == "<") {
    children.push_back(parse_template_parameter_decl_list(l));
  }
  PARSE_ASSERT_VAL(l, "=");
  children.push_back({ *l.next(), {} });
  children.push_back(parse_type_definition(l));
  return { nterm::type_declaration, children };
}
