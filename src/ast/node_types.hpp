/** Fns in this file are implemented in node_types_impl.hpp */

struct ast_node;
struct fn_arg;

using gen_f = Value* (Module *m, LLVMContext &ctx, IRBuilder<> &b, scope& s, const type* exp, bool lval);
using gen_as_type_f = type* (Module *m, LLVMContext &ctx, IRBuilder<> &b, const scope& s);

struct statement_list {
  std::vector<ast_node> children;
  /** True if the last item of this list of statements is an expression
      (missing a ';'). This is important, because if so that counts as the
      value of this whole block. */
  bool is_last_item_expr;
  statement_list(const parse_node&);
  statement_list() {};
  gen_f gen;
};

struct fn_declaration {
  nonstd::string_view name;
  std::vector<fn_arg> template_arg_list;
  std::vector<fn_arg> arg_list;
  /** Return type of this function - this is not nullable */
  ast_node* return_type;
  /** The actual body of this fn - this is nullable */
  ast_node* body;
  bool is_export : 1;
  bool is_mut : 1; // Does this function have mutable access to the struct it belongs to (assuming this is a method)
  fn_declaration(const parse_node&);
  fn_declaration() {};
  gen_f gen;
};

struct comptime {
  statement_list val;
};

struct unres_ident {
  nonstd::string_view val;
  gen_as_type_f gen_as_type;
};

struct float_lit {
  double val;
};

struct int_lit {
  int64_t val;
  gen_f gen;
};

struct bin_expr {
  ast_node* lchild;
  ast_node* rchild;
  bin_op op;
  gen_f gen;
};

struct ast_node {
  nonstd::variant<statement_list, bin_expr, unres_ident, float_lit, int_lit, comptime, fn_declaration> val;
  source_label sl;

private:
  void init(const parse_node& n);
public:

  ast_node(const parse_node& n);
  ast_node() {};

  gen_f gen;
  gen_as_type_f gen_as_type;
};

struct fn_arg {
  /** Optional, since we might not name the parameter,
      i.e. `fn add(i32, i32)` rather than `fn add(a: i32, b: i32)` */
  nonstd::optional<nonstd::string_view> name;
  ast_node type;
  fn_arg(const parse_node&);
  fn_arg() {};
};
