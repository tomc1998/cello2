parse_node parse_template_parameter_decl_list(lexer& l) {
  std::vector<parse_node> children;
  PARSE_ASSERT_VAL(l, "<");
  children.push_back({ *l.next(), {} });
  while(l.peek() && l.peek()->val != ">") {
    children.push_back(parse_parameter_decl(l, true));
    if (l.peek() && l.peek()->val != "," && l.peek()->val != ">") {
      throw parse_error(l, std::string("Expected ',' or '>', got ")
                        + std::string(l.peek()->val));
    }
    if (l.peek() && l.peek()->val == ",") {
      children.push_back({ *l.next(), {} });
    }
  }
  PARSE_ASSERT_VAL(l, ">");
  children.push_back({ *l.next(), {} });
  return { nterm::template_parameter_decl_list, children };
}
