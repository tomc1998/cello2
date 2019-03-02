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
