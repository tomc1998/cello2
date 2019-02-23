
parse_node parse_statement_list(lexer& l) {
  parse_error root;
  std::vector<parse_node> children;
  PARSE_ASSERT_VAL(l, "{");
  children.push_back({ *l.next(), {} });
  while(l.peek() && l.peek()->val != "}") {
    try {
      children.push_back(parse_statement(l));
      if (l.peek() && l.peek()->val == ";") {
        children.push_back({ *l.next(), {} });
      }
    } catch (parse_error e) {
      root.push_back(e);
      try_consume_until_after_item(l);
    }
  }
  if (root.children.size() > 0) {
    throw root;
  }
  PARSE_ASSERT_VAL(l, "}");
  children.push_back({ *l.next(), {} });
  return { nterm::statement_list, children };
}
