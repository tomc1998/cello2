/**
   Includes everything from the ast/ directory for converting the parse tree to
   an ast.
*/

#include <nonstd/optional.hpp>
#include <nonstd/string_view.hpp>
#include <mapbox/variant.hpp>
#include <iostream>

// Put variant into nonstd namespace, 'cause it's f u c k i n g annoying to have
// to type out mapbox::util:: all the time
namespace nonstd {
  using namespace mapbox::util;
}

/* LLVM shite */
// LLVM IR
#include <llvm/IR/Value.h>
#include <llvm/IR/Type.h>
#include <llvm/IR/Constant.h>
#include <llvm/IR/Constants.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/DerivedTypes.h>
// LLVM target init
#include <llvm/Support/TargetRegistry.h>
#include <llvm/Support/TargetSelect.h>
#include <llvm/Target/TargetMachine.h>
#include <llvm/Target/TargetOptions.h>

using namespace llvm;

#include "source_label.hpp"
#include "lexer.hpp"
#include "type.hpp"
#include "parser.hpp"
#include "scope.hpp"
#include "ast.hpp"

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
  // Implements typegen (similar to codegen for llvm::Type*) from an ast
#include "ast/typegen.hpp"
  // Implements codegen from an ast
#include "ast/codegen.hpp"

}
