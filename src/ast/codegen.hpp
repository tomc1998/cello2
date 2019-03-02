Value* int_lit::gen(Module *m, LLVMContext &ctx, IRBuilder<> &b, scope& s, const type* exp, bool lval) {
  if (exp) {
    return ConstantInt::get(exp->to_llvm_type(s, ctx), (uint64_t)val, true);
  } else {
    return ConstantInt::get(Type::getInt64Ty(ctx), (uint64_t)val, true);
  }
}

Value* bin_expr::gen(Module *m, LLVMContext &ctx, IRBuilder<> &b, scope& s, const type* exp, bool lval) {
  switch (op) {
  case bin_op::add:
    return b.CreateAdd(lchild->gen(m, ctx, b, s, exp, lval), rchild->gen(m, ctx, b, s, exp, lval));
  default:
    assert(false && "Unimpl");
  }
}

Value* fn_declaration::gen(Module *m, LLVMContext &ctx, IRBuilder<> &b, scope& s, const type* exp, bool lval) {
  // Create the function
  std::vector<Type*> types(arg_list.size(), nullptr);
  std::transform(arg_list.begin(), arg_list.end(), types.begin(),
                 [&](auto a) {return a.type.gen_as_type(m, ctx, b, s)->to_llvm_type(s, ctx);});
  const auto ft = FunctionType::get(return_type->gen_as_type(m, ctx, b, s)->to_llvm_type(s, ctx), types, false);
  const auto llvm_name = StringRef(name.begin(), name.size());
  const auto f = Function::Create(ft, Function::ExternalLinkage, llvm_name, m);
  const auto bb = BasicBlock::Create(ctx, "entry", f);
  b.SetInsertPoint(bb);
  // Loop over the arg list & add them all to the subscope
  auto subscope = s.create_subscope();
  for (unsigned ii = 0; ii < arg_list.size(); ++ii) {
    assert(arg_list[ii].name);
    const auto arg_name = *arg_list[ii].name;
    const auto arg_type = *arg_list[ii].type.gen_as_type(m, ctx, b, subscope);
    const auto arg_val = f->arg_begin() + ii;
    (f->arg_begin() + ii)->setName(StringRef(arg_name.begin(), arg_name.size()));
    subscope.symbol_table.insert(std::make_pair(arg_name, var { arg_type, arg_val, false, false }));
  }
  b.CreateRet(body->gen(m, ctx, b, subscope, return_type->gen_as_type(m, ctx, b, subscope), lval));
  return f;
}

Value* statement_list::gen(Module *m, LLVMContext &ctx, IRBuilder<> &b, scope& s, const type* exp, bool lval) {
  auto subscope = s.create_subscope();
  for (unsigned ii = 0; ii < children.size(); ++ii) {
    if (ii == children.size() - 1) {
      return children[ii].gen(m, ctx, b, subscope, exp, lval);
    } else {
      children[ii].gen(m, ctx, b, subscope, exp, lval);
    }
  }
  assert(false && "Empty statement list");
}

Value* unres_ident::gen(Module *m, LLVMContext &ctx, IRBuilder<> &b, scope& s, const type* exp, bool lval) {
  const auto res = s.find_symbol(val);
  if (!res) {
    std::cout << val << std::endl;
    assert(false && "Needs proper error here");
  }
  return res->val;
}

/**
   @param lval - True if this is an l value
*/
Value* ast_node::gen(Module *m, LLVMContext &ctx, IRBuilder<> &b, scope& s, const type* exp, bool lval) {
  if (val.template is<fn_declaration>()) {
    return val.template get<fn_declaration>().gen(m, ctx, b, s, exp, lval);
  } else if (val.template is<int_lit>()) {
    return val.template get<int_lit>().gen(m, ctx, b, s, exp, lval);
  } else if (val.template is<bin_expr>()) {
    return val.template get<bin_expr>().gen(m, ctx, b, s, exp, lval);
  } else if (val.template is<unres_ident>()) {
    return val.template get<unres_ident>().gen(m, ctx, b, s, exp, lval);
  } else if (val.template is<statement_list>()) {
    return val.template get<statement_list>().gen(m, ctx, b, s, exp, lval);
  }
  std::cout << "Unimplemented gen: " << to_string(*this) << std::endl;
  assert(false && "Unimpl");
}
