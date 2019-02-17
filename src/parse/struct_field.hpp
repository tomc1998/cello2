
parse_node parse_struct_field(lexer& l) {
  std::vector<parse_node> children;
  PARSE_ASSERT_NOT_EMPTY(l, "Expected struct field, found EOF");
  if (l.peek()->val == "static") {
    children.push_back({ *l.next(), {}});
  }
  PARSE_ASSERT_NOT_EMPTY(l, "Expected rest of struct field, found EOF");
  if (l.peek()->val == "fn") {
    children.push_back(parse_fn_declaration(l));
  } else {
    children.push_back(parse_member_var_decl(l));
  }
  return { nterm::struct_field, children };
}
