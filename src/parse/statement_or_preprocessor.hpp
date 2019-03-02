
parse_node parse_statement_or_preprocessor(lexer& l) {
  if (l.peek() && l.peek()->val == "#") {
    return { nterm::statement_or_preprocessor, { parse_preprocessor(l) }, l.get_curr_source_label() };
  } else {
    return { nterm::statement_or_preprocessor, { parse_statement(l) }, l.get_curr_source_label() };
  }
}
