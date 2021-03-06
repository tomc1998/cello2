parse_node parse_make_expression(lexer& l) {
  std::vector<parse_node> children;
  PARSE_ASSERT_VAL(l, "make");
  children.push_back({ *l.next(), {}, l.get_curr_source_label() });
  children.push_back(parse_expression(l));
  if (l.peek() && l.peek()->val == "(") {
    children.push_back(parse_parameter_list(l));
  } else {
    PARSE_ASSERT_VAL(l, "{");
    children.push_back({ *l.next(), {}, l.get_curr_source_label() });
    while(l.peek() && l.peek()->val != "}") {
      children.push_back(parse_expression(l));
      if (l.peek() && l.peek()->val == ",") {
        children.push_back({ *l.next(), {}, l.get_curr_source_label() });
      }
    }
    PARSE_ASSERT_VAL(l, "}");
    children.push_back({ *l.next(), {}, l.get_curr_source_label() });
  }
  return { nterm::make_expression, children, l.get_curr_source_label() };
}
