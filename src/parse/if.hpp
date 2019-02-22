parse_node parse_if(lexer& l) {
  std::vector<parse_node> children;
  PARSE_ASSERT_VAL(l, "if");
  children.push_back({ *l.next(), {} });
  children.push_back(parse_expression(l));
  children.push_back(parse_statement_list(l));
  while (l.peek() && l.peek()->val == "elif") {
    children.push_back(parse_elif(l));
  }
  if (l.peek() && l.peek()->val == "elif") {
    children.push_back(parse_else(l));
  }
  return { nterm::if_, children };
}
