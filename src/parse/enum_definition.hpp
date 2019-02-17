parse_node parse_enum_definition(lexer& l) {
  std::vector<parse_node> children;
  PARSE_ASSERT_VAL(l, "enum");
  children.push_back({ *l.next(), {} });
  PARSE_ASSERT_VAL(l, "{");
  children.push_back({ *l.next(), {} });
  while (l.peek() && l.peek()->val != "}") {
    children.push_back(parse_enum_field(l));
    if (l.peek() && l.peek()->val == ",") {
      children.push_back({ *l.next(), {} });
    }
  }
  PARSE_ASSERT_VAL(l, "}");
  children.push_back({ *l.next(), {} });
  return { nterm::enum_definition, children };
}
