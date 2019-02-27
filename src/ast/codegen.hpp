Value* int_lit::gen(Module *m, LLVMContext &ctx, IRBuilder<> &b, bool lval) {
  return ConstantInt::get(Type::getInt64Ty(ctx), (uint64_t)val, true);
}

Value* fn_declaration::gen(Module *m, LLVMContext &ctx, IRBuilder<> &b, bool lval) {
  // Create the function
  std::vector<Type*> types(arg_list.size(), nullptr);
  std::transform(arg_list.begin(), arg_list.end(), types.begin(),
                 [&](auto a) {return a.type.gen_as_type(m, ctx, b);});
  const auto ft = FunctionType::get(return_type->gen_as_type(m, ctx, b), types, false);
  const auto llvm_name = llvm::StringRef(name.begin(), name.size());
  const auto f = Function::Create(ft, Function::ExternalLinkage, llvm_name, m);
  assert(false && "Unimplemented function body gen");
  return f;
}

/**
   @param lval - True if this is an l value
*/
Value* ast_node::gen(Module *m, LLVMContext &ctx, IRBuilder<> &b, bool lval) {
  return val.match([&](statement_list e) {return (Value*)nullptr;},
                   [&](bin_expr e) {return (Value*)nullptr;},
                   [&](comptime e) {return (Value*)nullptr;},
                   [&](unres_ident e) {return (Value*)nullptr;},
                   [&](float_lit e) {return (Value*)nullptr;},
                   [&](fn_declaration e) {return e.gen(m, ctx, b, lval);},
                   [&](int_lit e) {return e.gen(m, ctx, b, lval);});
}

Type* ast_node::gen_as_type(Module *m, LLVMContext &ctx, IRBuilder<> &b) {
  return val.match([&](statement_list e) {return (Type*)nullptr;},
                   [&](bin_expr e) {return (Type*)nullptr;},
                   [&](comptime e) {return (Type*)nullptr;},
                   [&](unres_ident e) {return (Type*)nullptr;},
                   [&](float_lit e) {return (Type*)nullptr;},
                   [&](fn_declaration e) {return (Type*)nullptr;},
                   [&](int_lit e) {return (Type*)nullptr;});
}
