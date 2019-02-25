
ast_node* shunting_yard(const parse_node& root);
std::string to_string(const ast_node &val);

struct ast_node {
  nonstd::variant<statement_list, bin_expr, unres_ident, float_lit, int_lit, comptime> val;

private:
  void init(const parse_node& n) {
    if (n.is_nterm(nterm::program)) {
      val = statement_list(n);
    } else if (n.is_nterm(nterm::statement)) {
      if (n.children[0].is_nterm(nterm::expression)) {
        init(n.children[0]);
      } else {
        std::cerr << "Non-expr statement unsupported" << std::endl;
      }
    } else if (n.is_nterm(nterm::comptime)) {
      val = comptime { statement_list(n.children[1]) };
    } else if (n.is_nterm(nterm::expression)) {
      init(n.children[0]);
    } else if (n.is_nterm(nterm::binary_expression)) {
      val = shunting_yard(n)->val;
    } else if (n.is_nterm(nterm::literal)) {
      const auto &tok = n.children[0].val.template get<token>();
      if (tok.type == token_type::int_lit) {
        val = int_lit { std::stoll(std::string(tok.val)) };
      } else if (tok.type == token_type::float_lit) {
        val = float_lit { std::stod(std::string(tok.val)) };
      } else {
        std::cerr << "Unsupported literal" << std::endl;
      }
    } else if (n.is_nterm(nterm::identifier)) {
      val = unres_ident {n.children[0].val.template get<token>().val};
    } else {
      std::cerr << "Unsupported parse node: ";
      if (n.val.template is<nterm>()) {
        std::cerr << "NTERM " << (int)n.val.template get<nterm>() << std::endl;
      } else {
        std::cerr << "TERM " << n.val.template get<token>().val << std::endl;
      }
    }
  }
public:

  ast_node() {};
  ast_node(const parse_node& n) {
    init(n);
  }

  llvm::Value* gen() {
    val.match([&](statement_list e) {},
              [&](bin_expr e) {},
              [&](comptime e) {},
              [&](unres_ident e) {},
              [&](float_lit e) {},
              [&](int_lit e) {});
    return nullptr;
  }
};
