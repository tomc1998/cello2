parse_node parse_qualified_name(lexer& l, parse_node lrec) {
  std::vector<parse_node> children { lrec };
  while (l.peek() && (l.peek()->val == "::" || l.peek()->val == ".")
         && (l.peek(1)->type == token_type::ident
             || l.peek(1)->type == token_type::int_lit)) {
    children.push_back({ *l.next(), {} });
    if (l.peek() && l.peek()->type == token_type::int_lit) {
      children.push_back(parse_literal(l));
    } else {
      children.push_back(parse_identifier(l));
    }
  }
  return { nterm::qualified_name, children };
}
