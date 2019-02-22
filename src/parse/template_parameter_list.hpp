parse_node parse_template_parameter_list(lexer& l) {
  std::vector<parse_node> children;
  PARSE_ASSERT_VAL(l, "<");
  children.push_back({ *l.next(), {} });
  while(l.peek() && l.peek()->val != ">") {
    children.push_back(parse_expression(l, true));
    if (l.peek() && l.peek()->val == ",") {
      children.push_back({ *l.next(), {} });
    }
  }
  PARSE_ASSERT_VAL(l, ">");
  children.push_back({ *l.next(), {} });
  return { nterm::parameter_list, children };
}
