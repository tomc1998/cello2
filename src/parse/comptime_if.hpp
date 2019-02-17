parse_node parse_comptime_if(lexer& l) {
  std::vector<parse_node> children;
  PARSE_ASSERT_VAL(l, "comptime");
  children.push_back({ *l.next(), {}});
  children.push_back(parse_if(l));
  return { nterm::comptime_if, children };
}
