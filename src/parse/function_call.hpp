parse_node parse_function_call(lexer& l, parse_node lrec) {
  std::vector<parse_node> children { lrec };
  if (l.peek() && l.peek()->val == "::") {
    children.push_back({ *l.next(), {}, l.get_curr_source_label() });
    children.push_back(parse_template_parameter_list(l));
  }
  children.push_back(parse_parameter_list(l));
  return { nterm::function_call, children, l.get_curr_source_label() };
}
