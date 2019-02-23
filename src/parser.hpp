/**
   This file includes all the helpers in parse/, and uses them to parse an input
   stream of tokens.
   One parser per non-terminal.

   Most parse functions just take a lexer& and return a parse_node, but since
   left recursion exists in the grammar, some also take another parse node. For
   example, here's a snippet of some example grammar:

   Expression ::= Ident | FunctionCall
   FunctionCall ::= Expression "(" ParameterList ")"

   parse_expression would just take 1 param - `parse_expression(lexer& l)`
   but parse_function_call would take 2, since it's left recursive - `parse_function_call(lexer& l, parse_node lrec)`

   This would then end up as the following tree (in lispy syntax) `( expression ( function_call ( lrec "(" parameterlist ")" ) ) )`

   this way we can avoid infinite recursion. This holds for any other
   productions which are left recursive, and basically applies only to
   Expression and its children.
 */

/** Possible non-terminals */
enum class nterm {
  program,
  statement_or_preprocessor,
  preprocessor,
  statement_list,
  statement,
  expression,
  var_declaration,
  fn_declaration,
  extern_fn_declaration,
  fn_signature,
  fn_type,
  type_declaration,
  type_definition,
  struct_definition,
  struct_field,
  enum_definition,
  enum_field,
  member_var_decl,
  comptime,
  parameter_decl_list,
  template_parameter_decl_list,
  parameter_decl,
  parameter_list,
  template_parameter_list,
  meta_type_ident,
  identifier,
  literal,
  make_expression,
  op,
  if_,
  elif,
  else_,
  lambda,
  for_loop,
  comptime_for_loop,
  comptime_if,
  qualified_name,
  qualified_type,
  binary_expression,
  function_call,
  array_access,
  empty_array_access,
  range,
  assignment,
};

/** A parse node is either a terminal (just a string_view) or a non-terminal. It
    also contains children. */
struct parse_node {
  nonstd::variant<nterm, token> val;
  std::vector<parse_node> children;
};

struct parse_tree {
  parse_node root;
};

// Forward declare all the parse functions
parse_node parse_program(lexer& l);
parse_node parse_statement_or_preprocessor(lexer& l);
parse_node parse_preprocessor(lexer& l);
parse_node parse_statement_list(lexer& l);
parse_node parse_statement(lexer& l);
parse_node parse_expression(lexer& l, bool no_right_angle = false);
parse_node parse_var_declaration(lexer& l);
parse_node parse_fn_declaration(lexer& l);
parse_node parse_extern_fn_declaration(lexer& l);
parse_node parse_fn_signature(lexer& l);
parse_node parse_fn_type(lexer& l);
parse_node parse_type_declaration(lexer& l);
parse_node parse_type_definition(lexer& l);
parse_node parse_struct_definition(lexer& l);
parse_node parse_struct_field(lexer& l);
parse_node parse_enum_definition(lexer& l);
parse_node parse_enum_field(lexer& l);
parse_node parse_member_var_decl(lexer& l);
parse_node parse_comptime(lexer& l);
parse_node parse_parameter_decl_list(lexer& l);
parse_node parse_template_parameter_decl_list(lexer& l);
parse_node parse_parameter_decl(lexer& l, bool no_right_angle = false);
parse_node parse_parameter_list(lexer& l);
parse_node parse_template_parameter_list(lexer& l);
parse_node parse_meta_type_ident(lexer& l);
parse_node parse_identifier(lexer& l);
parse_node parse_literal(lexer& l);
parse_node parse_make_expression(lexer& l);
parse_node parse_op(lexer& l);
parse_node parse_for_loop(lexer& l);
parse_node parse_comptime_for_loop(lexer& l);
parse_node parse_if(lexer& l);
parse_node parse_comptime_if(lexer& l);
parse_node parse_elif(lexer& l);
parse_node parse_else(lexer& l);
parse_node parse_lambda(lexer& l);
parse_node parse_qualified_name(lexer& l, parse_node lrec);
parse_node parse_qualified_type(lexer& l, parse_node lrec);
parse_node parse_binary_expression(lexer& l, parse_node lrec);
parse_node parse_function_call(lexer& l, parse_node lrec);
parse_node parse_array_access(lexer& l, parse_node lrec);
parse_node parse_empty_array_access(lexer& l, parse_node lrec);
parse_node parse_range(lexer& l, parse_node lrec);
parse_node parse_assignment(lexer& l, parse_node lrec);

// Include parse util
#include "parse/util.hpp"

// Include all parsers
#include "parse/program.hpp"
#include "parse/statement_or_preprocessor.hpp"
#include "parse/preprocessor.hpp"
#include "parse/statement_list.hpp"
#include "parse/statement.hpp"
#include "parse/expression.hpp"
#include "parse/var_declaration.hpp"
#include "parse/fn_declaration.hpp"
#include "parse/extern_fn_declaration.hpp"
#include "parse/fn_signature.hpp"
#include "parse/fn_type.hpp"
#include "parse/type_declaration.hpp"
#include "parse/type_definition.hpp"
#include "parse/struct_definition.hpp"
#include "parse/struct_field.hpp"
#include "parse/enum_definition.hpp"
#include "parse/enum_field.hpp"
#include "parse/member_var_decl.hpp"
#include "parse/comptime.hpp"
#include "parse/parameter_decl_list.hpp"
#include "parse/template_parameter_decl_list.hpp"
#include "parse/parameter_decl.hpp"
#include "parse/parameter_list.hpp"
#include "parse/template_parameter_list.hpp"
#include "parse/meta_type_ident.hpp"
#include "parse/identifier.hpp"
#include "parse/literal.hpp"
#include "parse/make_expression.hpp"
#include "parse/op.hpp"
#include "parse/for_loop.hpp"
#include "parse/comptime_for_loop.hpp"
#include "parse/if.hpp"
#include "parse/elif.hpp"
#include "parse/else.hpp"
#include "parse/comptime_if.hpp"
#include "parse/lambda.hpp"
#include "parse/qualified_name.hpp"
#include "parse/qualified_type.hpp"
#include "parse/binary_expression.hpp"
#include "parse/function_call.hpp"
#include "parse/array_access.hpp"
#include "parse/empty_array_access.hpp"
#include "parse/range.hpp"
#include "parse/assignment.hpp"

parse_tree parse(lexer& l) {
  return { parse_program(l) };
}
