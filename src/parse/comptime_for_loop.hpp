parse_node parse_comptime_for_loop(lexer& l) {
  std::vector<parse_node> children;
  PARSE_ASSERT_VAL(l, "comptime");
  children.push_back({ *l.next(), {}});
  children.push_back(parse_for_loop(l));
  return { nterm::comptime_for_loop, children };
}
