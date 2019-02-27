/**
   Includes everything from the ast/ directory for converting the parse tree to
   an ast.
 */

namespace ast {

  // Binary operators
#include "ast/bin_op.hpp"
  // Contains definitions of AST nodes
#include "ast/node_types.hpp"
  // Shunting yard implementation for converting a parse subtree to an AST
  // w.r.t. operator precedence
#include "ast/shunting_yard.hpp"
  // Functions to print AST nodes
#include "ast/to_string.hpp"
  // Implements functions to convert from parse tree to AST nodes
#include "ast/node_types_convert_impl.hpp"
  // Implements codegen from an ast
#include "ast/codegen.hpp"

}
