struct ast_node;

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

