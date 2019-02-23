/**
   @param no_right_angle - When true, this won't parse right angle braces (">")
   as a greater than symbol. This is to prevent abiguity when declaring function
   template parameters, i.e. `foo<a: $type> = ...` won't parse the last bit as
   `type > =`.

   Bit of a hack, but the alternative is a totally separate production, called
   'expression_no_ra' or something.

   This won't propogate to future expressions, i.e. any expressions contained in
   this one will consume as greater than (this is generally what you want,
   though).

   The default value for this is false.
 */
parse_node parse_expression(lexer& l, bool no_right_angle) {
  PARSE_ASSERT_NOT_EMPTY(l, "Expected expression, found EOF");
  parse_node lrec;
  // Try to parse all non-recursive stuff
  if (l.peek()->val == "{") { // StatementList
    lrec = { nterm::expression, { parse_statement_list(l) } };
  } else if (l.peek()->val == "(") {
    std::vector<parse_node> children { { *l.next(), {} }, parse_expression(l) };
    PARSE_ASSERT_VAL(l, ")");
    children.push_back({ *l.next(), {} });
    lrec = { nterm::expression, children };
  } else if (l.peek()->val == "comptime") {
    if (l.peek(1) && l.peek(1)->val == "if") {
      lrec = { nterm::expression, { parse_comptime_if(l) } };
    } else if (l.peek(1) && (l.peek(1)->val == "var" || l.peek(1)->val == "mut")) {
      lrec = { nterm::expression, { parse_var_declaration(l) } };
    } else {
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
    } else if (l.peek()->val == "(") {
      lrec = parse_function_call(l, lrec);
    } else if (l.peek()->val == "::" && l.peek(1) && l.peek(1)->val == "<") {
      lrec = parse_qualified_type(l, lrec);
    } else if (l.peek()->val == "[") {
      if (l.peek(1) && l.peek(1)->val == "]") {
        lrec = parse_empty_array_access(l, lrec);
      } else {
        lrec = parse_array_access(l, lrec);
      }
    } else if (l.peek()->val == "..") {
      lrec = parse_range(l, lrec);
    }
    // Check if this is a binary op, but also if no_right_angle is set, check it's not a ">".
    // As a bonus, don't parse assignment ops here
    else if (l.peek()->type == token_type::op && !is_assignment_op(*l.peek()) && (!no_right_angle || l.peek()->val != ">")) {
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
