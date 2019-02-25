/**
   Includes everything from the ast/ directory for converting the parse tree to
   an ast.
 */

#include <llvm/IR/Value.h>

namespace ast {

#include "ast/bin_op.hpp"
#include "ast/node_types.hpp"
#include "ast/ast_node.hpp"
#include "ast/shunting_yard.hpp"
#include "ast/to_string.hpp"
#include "ast/node_types_impl.hpp"

}
