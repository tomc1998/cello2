
parse_node parse_fn_signature(lexer& l) {
  std::vector<parse_node> children;
  children.push_back(parse_parameter_decl_list(l));
  if (l.peek() && l.peek()->val == "mut") {
    children.push_back({ *l.next(), {} });
  }
  if (l.peek() && l.peek()->val == "->") {
    children.push_back({ *l.next(), {} });
    children.push_back(parse_expression(l));
  }
  return { nterm::fn_signature, children };
}
