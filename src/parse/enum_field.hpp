parse_node parse_enum_field(lexer& l) {
  std::vector<parse_node> children;
  children.push_back(parse_identifier(l));
  if (l.peek() && l.peek()->val == "(") {
    children.push_back({ *l.next(), {}, l.get_curr_source_label() });
    children.push_back(parse_expression(l));
    PARSE_ASSERT_VAL(l, ")");
    children.push_back({ *l.next(), {}, l.get_curr_source_label() });
  }
  return { nterm::enum_field, children, l.get_curr_source_label() };
}
