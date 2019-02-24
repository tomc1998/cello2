#include <llvm/IR/Value.h>

namespace ast {

  struct ast_node;

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
    /** Sorts this node and all children given precedence */
    void sort_with_precendence() {
    }
  };

  std::string to_string(const ast_node &val);

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
          val = shunting_yard(n)->val;
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

    /**
       Similar to init, but does shunting yard on a binary expression.
       @param n - An expression, who's first child is a binary expression.
    */
    ast_node* shunting_yard(const parse_node& root) {
      std::vector<bin_op> operator_stack;
      std::vector<ast_node*> output_stack;
      std::vector<const parse_node*> visit_queue { &root.children[0] };

      // Pops from the operator stack and applies the op to the top of the
      // output stack
      const auto consume_op = [&]() {
        // create bin expr struct
        const auto lchild = output_stack[output_stack.size()-2];
        const auto rchild = output_stack[output_stack.size()-1];
        auto this_op = operator_stack[operator_stack.size()-1];
        output_stack.pop_back();
        output_stack.pop_back();
        operator_stack.pop_back();
        bin_expr e {lchild, rchild, this_op};
        // Create actual ast node
        auto ast_expr = new ast_node();
        ast_expr->val = e;
        return ast_expr;
      };

      const auto process = [&](const auto n) {
        // If op, push to stack, otherwise add to output list
        if (n->is_nterm(nterm::op)) {
          bin_op o = bin_op_from_parse_node(n->children[0].val
                                            .template get<token>().val);
          while (operator_stack.size() > 0 &&
                 bin_op_precedence(operator_stack[operator_stack.size()-1])
                 > bin_op_precedence(o)) {
            output_stack.push_back(consume_op());
          }
          operator_stack.push_back(o);
        } else {
          output_stack.push_back(new ast_node(*n));
        }
      };

      while (visit_queue.size() > 0) {
        // Extract next node
        const auto curr_node = visit_queue[0];
        visit_queue.erase(visit_queue.begin());
        for (const auto& n : curr_node->children) {
          if (n.children[0].is_nterm(nterm::binary_expression)) {
            visit_queue.push_back(&n.children[0]);
          } else {
            process(&n);
          }
        }
      }

      while (operator_stack.size() > 0) {
        output_stack.push_back(consume_op());
      }

      return output_stack[0];
    }

    ast_node() {};

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
