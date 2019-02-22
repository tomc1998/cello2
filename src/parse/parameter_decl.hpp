/**
   @param no_right_angle - See expression.hpp comment.
 */
parse_node parse_parameter_decl(lexer& l, bool no_right_angle) {
  std::vector<parse_node> children;
  if (l.peek(0) && l.peek(1) && l.peek(0)->type == token_type::ident
      && l.peek(1)->val == ":") {
    children.push_back(parse_identifier(l));
    PARSE_ASSERT_VAL(l, ":");
    children.push_back({ *l.next(), {} });
    children.push_back(parse_expression(l, no_right_angle));
  } else {
    children.push_back(parse_expression(l, no_right_angle));
  }
  return { nterm::parameter_decl, children };
}
