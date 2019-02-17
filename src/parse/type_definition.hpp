
parse_node parse_type_definition(lexer& l) {
  PARSE_ASSERT_NOT_EMPTY(l, "Expected type definition, found EOF");
  if (l.peek()->val == "comptime") {
    return { nterm::type_definition, { parse_comptime(l) } };
  } else if (l.peek()->val == "struct") {
    return { nterm::type_definition, { parse_struct_definition(l) } };
  } else if (l.peek()->val == "enum") {
    return { nterm::type_definition, { parse_enum_definition(l) } };
  } else {
    return { nterm::type_definition, { parse_expression(l) } };
  }
}
