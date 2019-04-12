
#include <nonstd/string_view.hpp>
#include <nonstd/optional.hpp>
#include <mapbox/variant.hpp>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <vector>
#include <cstdint>

#include "source_label.hpp"
#include "lexer.hpp"
#include "parse_error.hpp"
#include "parser.hpp"

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
