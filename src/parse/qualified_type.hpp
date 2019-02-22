parse_node parse_qualified_type(lexer& l, parse_node lrec) {
  std::vector<parse_node> children { lrec };
  PARSE_ASSERT_VAL(l, "::");
  children.push_back({ *l.next(), {} });
  children.push_back(parse_template_parameter_list(l));
  return { nterm::qualified_type, children };
}
