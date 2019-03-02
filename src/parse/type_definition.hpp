
parse_node parse_type_definition(lexer& l) {
  PARSE_ASSERT_NOT_EMPTY(l, "Expected type definition, found EOF");
  if (l.peek()->val == "comptime") {
    return { nterm::type_definition, { parse_comptime(l) }, l.get_curr_source_label() };
  } else if (l.peek()->val == "struct") {
    return { nterm::type_definition, { parse_struct_definition(l) }, l.get_curr_source_label() };
  } else if (l.peek()->val == "enum") {
    return { nterm::type_definition, { parse_enum_definition(l) }, l.get_curr_source_label() };
  } else {
    return { nterm::type_definition, { parse_expression(l) }, l.get_curr_source_label() };
  }
}
