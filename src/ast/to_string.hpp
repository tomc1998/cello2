std::string to_string(const ast_node &val);

std::string to_string(const statement_list &v) {
  std::string res = "statement_list(";
  for (const auto& c : v.children) {
    res += to_string(c) + ", ";
  }
  return res + ")";
}

std::string to_string(const bin_op &v) {
  switch (v) {
  case bin_op::add: return "add";
  case bin_op::sub: return "sub";
  case bin_op::div: return "div";
  case bin_op::mul: return "mul";
  case bin_op::gt: return "gt";
  case bin_op::ge: return "ge";
  case bin_op::lt: return "lt";
  case bin_op::le: return "le";
  case bin_op::eq: return "eq";
  case bin_op::mod: return "mod";
  case bin_op::land: return "land";
  case bin_op::lor: return "lor";
  case bin_op::band: return "band";
  case bin_op::bor: return "bor";
  case bin_op::bxor: return "bxor";
  }
  assert(false);
}

std::string to_string(const fn_declaration &v) {
  std::string res = std::string("fn_declaration(") + to_string(v.name) + "(";
  for (const auto &arg : v.arg_list) {
    if (arg.name) {
      res += std::string(*arg.name) + ": " + to_string(arg.type) + ", ";
    } else {
      res += to_string(arg.type) + ", ";
    }
  }
  if (v.body) {
    res += "), " + to_string(*v.body) + ")";
  } else {
    res += "))";
  }
  return res;
}

std::string to_string(const bin_expr &v) {
  std::string res = std::string("bin_expr(") + to_string(*v.lchild)
    + ", " + to_string(*v.rchild) + ", " + to_string(v.op) + ")";
  return res;
}

std::string to_string(const ast_node &val) {
  return val.val.match([](statement_list v){return to_string(v);},
                       [](bin_expr v){return to_string(v);},
                       [](comptime v){return "comptime(" + to_string(v.val) + ")";},
                       [](unres_ident v){return std::string(v.val);},
                       [](float_lit v){return std::to_string(v.val);},
                       [](int_lit v){return std::to_string(v.val);},
                       [](fn_declaration v){return to_string(v);}
                       );
}
