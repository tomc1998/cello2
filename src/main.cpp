#include <nonstd/optional.hpp>
#include <nonstd/string_view.hpp>
#include <mapbox/variant.hpp>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <cstdint>
#include <regex>

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

// Only including this to throw runtime_errors. Can be much better than asserts.
#include <exception>

// Put variant into nonstd namespace, 'cause it's f u c k i n g annoying to have
// to type out mapbox::util:: all the time
namespace nonstd {
  using namespace mapbox::util;
}

#include "arg_parse.hpp"
#include "source_label.hpp"
#include "lexer.hpp"
#include "type.hpp"
#include "scope.hpp"
#include "parse_error.hpp"
#include "parser.hpp"
#include "builtin_types.hpp"
#include "ast.hpp"

int main(int argc, const char** argv) {

  const auto args = parse_prog_arg_list(argc, argv);
  if (args.num_positional() == 0) {
    std::cerr << "Please pass a file to compile" << std::endl;
    return 1;
  } else if (args.num_positional() > 1) {
    std::cerr << "2 positional args passed - compiler only supports compiling 1 file" << std::endl;
    return 1;
  }
  auto file_name = std::string(args.get_positional(0).name);
  std::ifstream t(file_name);
  if (!t.good()) {
    std::cerr << "Can't read file '" << file_name << "'" << std::endl;
    return 1;
  }
  std::string file((std::istreambuf_iterator<char>(t)),
                   std::istreambuf_iterator<char>());

  lexer l(file_name, file);
  const auto tree = parse(l);
  ast::ast_node ast(tree);

  // Code gen the ast
  InitializeAllTargetInfos();
  InitializeAllTargets();
  InitializeAllTargetMCs();
  InitializeAllAsmParsers();
  InitializeAllAsmPrinters();

  LLVMContext ctx;
  IRBuilder<> builder(ctx);
  std::unique_ptr<Module> module = make_unique<Module>("test_mod", ctx);
  scope module_scope;
  add_builtin_types(module_scope, ctx);
  const auto val = ast.gen(module.get(), ctx, builder, module_scope, nullptr, false);
  val->print(errs());

  std::cout << "AST" << std::endl;
  std::cout << ast::to_string(ast) << std::endl;


  return 0;
}
