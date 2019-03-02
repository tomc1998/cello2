parse_node parse_fn_declaration(lexer& l) {
  std::vector<parse_node> children;
  if (l.peek()->val == "export") {
    children.push_back({ *l.next(), {}, l.get_curr_source_label() });
  }
  PARSE_ASSERT_VAL(l, "fn");
  children.push_back({ *l.next(), {}, l.get_curr_source_label() });
  children.push_back(parse_identifier(l));
  PARSE_ASSERT_NOT_EMPTY(l, "Expected rest of function declaration, got EOF");
  if (l.peek()->val == "<") {
    children.push_back(parse_template_parameter_decl_list(l));
  }
  PARSE_ASSERT_VAL(l, "=");
  children.push_back({ *l.next(), {}, l.get_curr_source_label() });
  children.push_back(parse_fn_signature(l));
  children.push_back(parse_expression(l));

  return { nterm::fn_declaration, children, l.get_curr_source_label() };
}
