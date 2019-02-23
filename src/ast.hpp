#include <llvm/IR/Value.h>

namespace ast {

  struct ast_node;

  enum class bin_op {
    add, sub, div, mul, gt, ge, lt, le, eq
  };

  bin_op bin_op_from_parse_node(const parse_node& n) {
    std::cerr << "Unimplemented binop parsing, default add" << std::endl;
    return bin_op::add;
  }

  struct statement_list {
    std::vector<ast_node> children;
    /** True if the last item of this list of statements is an expression
        (missing a ';'). This is important, because if so that counts as the
        value of this whole block. */
    bool is_last_item_expr;
  };

  struct ident {
    nonstd::string_view val;
  };

  struct float_lit {
    double val;
  };

  struct int_lit {
    int64_t val;
  };

  struct bin_expr {
    ast_node* lchild;
    ast_node* rchild;
    bin_op op;
  };

  struct ast_node {
    nonstd::variant<statement_list, bin_expr, ident, float_lit, int_lit> val;

  private:
    void init(const parse_node& n) {
      if (n.is_nterm(nterm::program)) {
        std::vector<ast_node> children;
        for (const auto c : n.children) {
          if (c.is_nterm(nterm::statement_or_preprocessor)) {
            const auto& s = c.children[0];
            if (s.is_nterm(nterm::preprocessor)) {
              std::cerr << "Preprocessor unsupported" << std::endl;
            }
            children.push_back(ast_node(s));
          }
        }
        val = statement_list{
          children,
          n.children[n.children.size()-2].is_term_val(";")
        };
      } else if (n.is_nterm(nterm::statement)) {
        if (n.children[0].is_nterm(nterm::expression)) {
          init(n.children[0]);
        } else {
          std::cerr << "Non-expr statement unsupported" << std::endl;
        }
      } else if (n.is_nterm(nterm::expression)) {
        if (n.children[0].is_nterm(nterm::binary_expression)) {
          const auto lchild = new ast_node(n.children[0].children[0]);
          const auto op = bin_op_from_parse_node(n.children[0].children[1]);
          const auto rchild = new ast_node(n.children[0].children[2]);
          val = bin_expr {lchild, rchild, op};
        } else if (n.children[0].is_nterm(nterm::literal)) {
          const auto &tok = n.children[0].children[0].val.template get<token>();
          if (tok.type == token_type::int_lit) {
            val = int_lit { std::stoll(std::string(tok.val)) };
          } else if (tok.type == token_type::float_lit) {
            val = float_lit { std::stod(std::string(tok.val)) };
          } else {
            std::cerr << "Unsupported literal" << std::endl;
          }
        } else if (n.children[0].is_nterm(nterm::identifier)) {
        } else {
          std::cerr << "Unsupported expression type" << std::endl;
        }
      } else {
        std::cerr << "Unsupported parse node" << std::endl;
        if (n.val.template is<nterm>()) {
          std::cerr << "    NTERM " << (int)n.val.template get<nterm>() << std::endl;
        } else {
          std::cerr << "    TERM " << n.val.template get<token>().val << std::endl;
        }
      }
    }
  public:

    ast_node(const parse_node& n) {
      init(n);
    }

    llvm::Value* gen() {
      val.match([&](statement_list e) {},
                [&](bin_expr e) {},
                [&](ident e) {},
                [&](float_lit e) {},
                [&](int_lit e) {});
      return nullptr;
    }
  };

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
    }
    assert(false);
  }

  std::string to_string(const bin_expr &v) {
    std::string res = std::string("bin_expr(") + to_string(*v.lchild)
      + ", " + to_string(*v.rchild) + ", " + to_string(v.op) + ")";
    return res;
  }

  std::string to_string(const ast_node &val) {
    return val.val.match([](statement_list v){return to_string(v);},
                         [](bin_expr v){return to_string(v);},
                         [](ident v){return std::string("ident");},
                         [](float_lit v){return std::to_string(v.val);},
                         [](int_lit v){return std::to_string(v.val);});
  }
}
