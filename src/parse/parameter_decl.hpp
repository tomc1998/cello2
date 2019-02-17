parse_node parse_parameter_decl(lexer& l) {
  std::vector<parse_node> children;
  if (l.peek(0) && l.peek(1) && l.peek(0)->type == token_type::ident
      && l.peek(1)->val == ":") {
    children.push_back(parse_identifier(l));
    PARSE_ASSERT_VAL(l, ":");
    children.push_back({ *l.next(), {} });
    children.push_back(parse_expression(l));
  } else {
    children.push_back(parse_expression(l));
  }
  return { nterm::parameter_decl, children };
}
