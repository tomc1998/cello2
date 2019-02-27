
llvm::Value* ast_node::gen() {
  val.match([&](statement_list e) {},
            [&](bin_expr e) {},
            [&](comptime e) {},
            [&](unres_ident e) {},
            [&](float_lit e) {},
            [&](fn_declaration e) {},
            [&](int_lit e) {});
  return nullptr;
}
