
parse_node parse_member_var_decl(lexer& l) {
  std::vector<parse_node> children;
  PARSE_ASSERT_TYPE(l, token_type::ident, "identifier");
  children.push_back(parse_identifier(l));
  PARSE_ASSERT_VAL(l, ":");
  children.push_back({ *l.next(), {} });
  children.push_back(parse_expression(l));
  if (l.peek() && l.peek()->val == "{") {
    children.push_back({ *l.next(), {} });
    children.push_back(parse_literal(l));
    PARSE_ASSERT_VAL(l, "}");
    children.push_back({ *l.next(), {} });
  }
  if (l.peek() && l.peek()->val == "=") {
    children.push_back({ *l.next(), {} });
    children.push_back(parse_expression(l));
  }
  return { nterm::member_var_decl, children };
}
