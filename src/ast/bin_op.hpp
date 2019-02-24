  enum class bin_op {
    add, sub, div, mul, gt, ge, lt, le, eq, mod,
    // Logical
    land, lor,
    // Bitwise
    band, bor, bxor
  };

  inline int bin_op_precedence(const bin_op& o) {
    switch (o) {
    case bin_op::div: case bin_op::mul: case bin_op::band: case bin_op::bor:
    case bin_op::bxor:
      return 3;
    case bin_op::add: case bin_op::sub: case bin_op::mod:
      return 2;
    case bin_op::gt: case bin_op::ge: case bin_op::lt: case bin_op::le:
    case bin_op::eq:
      return 1;
    case bin_op::land: case bin_op::lor:
      return 0;
    }
    assert(false);
  }

  bin_op bin_op_from_parse_node(nonstd::string_view n) {
    if (n == "+")  { return bin_op::add; }
    if (n == "-")  { return bin_op::sub; }
    if (n == "/")  { return bin_op::div; }
    if (n == "*")  { return bin_op::mul; }
    if (n == ">")  { return bin_op::gt; }
    if (n == ">=") { return bin_op::ge; }
    if (n == "<")  { return bin_op::lt; }
    if (n == "<=") { return bin_op::le; }
    if (n == "==") { return bin_op::eq; }
    assert(false && "Unimplemented binop");
  }

