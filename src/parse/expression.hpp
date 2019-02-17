parse_node parse_expression(lexer& l) {
  PARSE_ASSERT_NOT_EMPTY(l, "Expected expression, found EOF");
  parse_node lrec;
  // Try to parse all non-recursive stuff
  if (l.peek()->val == "{") { // StatementList
    lrec = { nterm::expression, { parse_statement_list(l) } };
  } else if (l.peek()->val == "comptime") { // comptime or comptime fn
    if (l.peek(1) && l.peek(1)->val == "fn") { // comptime fn
      lrec = { nterm::expression, { parse_comptime_fn_declaration(l) } };
    } else if (l.peek(1) && l.peek(1)->val == "if") {
      lrec = { nterm::expression, { parse_comptime_if(l) } };
    } else { // comptime
      lrec = { nterm::expression, { parse_comptime(l) } };
    }
  } else if (l.peek()->val == "$") {
    lrec = { nterm::expression, { parse_meta_type_ident(l) } };
  } else if (l.peek()->type == token_type::op) {
    lrec = { nterm::expression, { parse_op(l), parse_expression(l) } };
  } else if (l.peek()->val == "fn") {
    lrec = { nterm::expression, { parse_fn_type(l) } };
  } else if (l.peek()->val == "lambda") {
    lrec = { nterm::expression, { parse_lambda(l) } };
  } else if (l.peek()->val == "if") {
    lrec = { nterm::expression, { parse_if(l) } };
  } else if (l.peek()->val == "make") {
    lrec = { nterm::expression, { parse_make_expression(l) } };
  } else if (l.peek()->type == token_type::int_lit ||
             l.peek()->type == token_type::float_lit ||
             l.peek()->type == token_type::string_lit ||
             l.peek()->type == token_type::c_string_lit) {
    // Literal
    lrec = { nterm::expression, { parse_literal(l) } };
  } else if (l.peek()->type == token_type::ident) {
    lrec = { nterm::expression, { parse_identifier(l) } };
  } else if (l.peek()->val == "undefined") {
    lrec = { nterm::expression, { { *l.next(), {} } } };
  } else {
    throw parse_error(l, std::string("Unable to parse expression starting with '")
                      + std::string(l.peek()->val) + "'");
  }

  // Continually look for stuff on the end to add until we've run out of tokens
  // - this is to prevent left recursion.
  bool added_lrec = false;
  while(true) {
    if (!l.peek()) {
      break;
    } else if ((l.peek()->val == "." || l.peek()->val == "::") && l.peek(1)
               && (l.peek(1)->type == token_type::ident
                   || l.peek(1)->type == token_type::int_lit)) {
      lrec = parse_qualified_name(l, lrec);
    } else if (l.peek()->val == "(" || (l.peek()->val == "::" && l.peek(1) && l.peek(1)->val == "<")) {
      lrec = parse_function_call(l, lrec);
    } else if (l.peek()->type == token_type::op) {
      lrec = parse_binary_expression(l, lrec);
    } else {
      break;
    }
    added_lrec = true;
  }

  // If we added some more stuff on the end in the previous stage, re-wrap this
  // in an 'expression' nterm. Otherwise, just return as-is.
  if (added_lrec) {
    return { nterm::expression,  { lrec } };
  } else {
    return lrec;
  }
}
