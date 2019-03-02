parse_node parse_binary_expression(lexer& l, parse_node lrec) {
  return { nterm::binary_expression, {lrec, parse_op(l), parse_expression(l)}, l.get_curr_source_label() };
}
