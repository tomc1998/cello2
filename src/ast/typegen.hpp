type* unres_ident::gen_as_type(Module *m, LLVMContext &ctx, IRBuilder<> &b, const scope& s) {
  const auto res = s.find_symbol(val);
  if (!res) {
    assert(false && "Needs proper error here");
  } else if (!res->var_type.val.template is<kind_type>()) {;
    assert(false && "Needs proper error here");
  }
  return res->var_type.val.template get<kind_type>().val;
}

/** The equivalent for codegen.hpp, but for generating LLVM types from internal
    types. */
type* ast_node::gen_as_type(Module *m, LLVMContext &ctx, IRBuilder<> &b, const scope& s) {
  if (val.template is<unres_ident>()) {
    return val.template get<unres_ident>().gen_as_type(m, ctx, b, s);
  }
  assert(false && "Unimpl");
}
