
parse_node parse_struct_definition(lexer& l) {
  std::vector<parse_node> children;
  PARSE_ASSERT_VAL(l, "struct");
  children.push_back({ *l.next(), {}, l.get_curr_source_label() });
  PARSE_ASSERT_VAL(l, "{");
  children.push_back({ *l.next(), {}, l.get_curr_source_label() });
  while (l.peek() && l.peek()->val != "}") {
    children.push_back(parse_struct_field(l));
    if (l.peek() && l.peek()->val != "," && l.peek()->val != "}") {
      throw parse_error(l, std::string("Expected ',' or '}', got '")
                        + std::string(l.peek()->val) + "'");
    } else if (l.peek() && l.peek()->val == ",") {
      children.push_back({ *l.next(), {}, l.get_curr_source_label() });
    }
  }
  PARSE_ASSERT_VAL(l, "}");
  children.push_back({ *l.next(), {}, l.get_curr_source_label() });
  return { nterm::struct_definition, children, l.get_curr_source_label() };
}
