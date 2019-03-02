
parse_node parse_meta_type_ident(lexer& l) {
  std::vector<parse_node> children;
  PARSE_ASSERT_VAL(l, "$");
  children.push_back({ *l.next(), {}, l.get_curr_source_label() });
  PARSE_ASSERT_NOT_EMPTY(l, "Expected meta type (i.e. '$type', '$expr', etc.), found EOF");
  children.push_back(parse_identifier(l));
  return { nterm::meta_type_ident, children, l.get_curr_source_label() };
}
