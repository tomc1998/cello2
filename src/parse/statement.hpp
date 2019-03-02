
parse_node parse_statement(lexer& l) {
  PARSE_ASSERT_NOT_EMPTY(l, "Expected statement, got EOF");
  std::vector<parse_node> children;
  if (l.peek()->val == "export") {
    if (l.peek(1) && (l.peek(1)->val == "var" || l.peek(1)->val == "mut")) {
      children.push_back(parse_var_declaration(l));
    } else if (l.peek(1) && l.peek(1)->val == "type") {
      children.push_back(parse_type_declaration(l));
    } else if (l.peek(1) && l.peek(1)->val == "fn") {
      children.push_back(parse_fn_declaration(l));
    } else {
      throw parse_error(l, std::string("Expected 'var', 'type', or 'fn'"
                                       " keywords after 'export', found ")
                        + std::string(l.peek()->val));
    }
  } else {
    if (l.peek()->val == "var" || l.peek()->val == "mut") {
      children.push_back(parse_var_declaration(l));
    } else if (l.peek()->val == "type") {
      children.push_back(parse_type_declaration(l));
    } else if (l.peek()->val == "fn") {
      children.push_back(parse_fn_declaration(l));
    } else if (l.peek()->val == "extern") {
      children.push_back(parse_extern_fn_declaration(l));
    } else if (l.peek()->val == "for") {
      children.push_back(parse_for_loop(l));
    } else if (l.peek()->val == "comptime" && l.peek(1) && l.peek(1)->val == "for") {
      children.push_back(parse_comptime_for_loop(l));
    } else {
      children.push_back(parse_expression(l));
    }
  }

  return { nterm::statement, children, l.get_curr_source_label() };
}
