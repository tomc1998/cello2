parse_node parse_enum_definition(lexer& l) {
  std::vector<parse_node> children;
  PARSE_ASSERT_VAL(l, "enum");
  children.push_back({ *l.next(), {}, l.get_curr_source_label() });
  PARSE_ASSERT_VAL(l, "{");
  children.push_back({ *l.next(), {}, l.get_curr_source_label() });
  while (l.peek() && l.peek()->val != "}") {
    children.push_back(parse_enum_field(l));
    if (l.peek() && l.peek()->val == ",") {
      children.push_back({ *l.next(), {}, l.get_curr_source_label() });
    }
  }
  PARSE_ASSERT_VAL(l, "}");
  children.push_back({ *l.next(), {}, l.get_curr_source_label() });
  return { nterm::enum_definition, children, l.get_curr_source_label() };
}
