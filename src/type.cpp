#include <nonstd/optional.hpp>
#include <nonstd/string_view.hpp>
#include <mapbox/variant.hpp>

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

Type* int_type::to_llvm_type(const scope& s, LLVMContext &c) const {
  return IntegerType::get(c, num_bits);
}

Type* float_type::to_llvm_type(const scope& s, LLVMContext &c) const {
  switch (num_bits) {
  case 32:
    return Type::getFloatTy(c);
  case 64:
    return Type::getDoubleTy(c);
  default:
    assert(false && "Unimpl");
  }
}

Type* type::to_llvm_type(const scope& s, LLVMContext &c) const {
  if (val.template is<int_type>()) {
    return val.template get<int_type>().to_llvm_type(s, c);
  } else if (val.template is<float_type>()) {
    return val.template get<float_type>().to_llvm_type(s, c);
  } else {
    assert(false && "Unimpl");
  }
}
