parse_node parse_extern_fn_declaration(lexer& l) {
  std::vector<parse_node> children;
  PARSE_ASSERT_VAL(l, "extern");
  children.push_back({ *l.next(), {}, l.get_curr_source_label() });
  PARSE_ASSERT_VAL(l, "fn");
  children.push_back({ *l.next(), {}, l.get_curr_source_label() });
  children.push_back(parse_identifier(l));
  PARSE_ASSERT_VAL(l, "=");
  children.push_back({ *l.next(), {}, l.get_curr_source_label() });
  children.push_back(parse_fn_signature(l));
  return { nterm::extern_fn_declaration, children, l.get_curr_source_label() };
}
