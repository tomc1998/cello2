parse_node parse_lambda(lexer& l) {
  std::vector<parse_node> children;
  PARSE_ASSERT_VAL(l, "lambda");
  children.push_back({ *l.next(), {}, l.get_curr_source_label() });
  children.push_back(parse_fn_signature(l));
  children.push_back(parse_expression(l));
  return { nterm::lambda, children, l.get_curr_source_label() };
}
