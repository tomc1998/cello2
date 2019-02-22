parse_node parse_elif(lexer& l) {
  std::vector<parse_node> children;
  PARSE_ASSERT_VAL(l, "elif");
  children.push_back({ *l.next(), {} });
  children.push_back(parse_expression(l));
  children.push_back(parse_statement_list(l));
  return { nterm::elif, children };
}
