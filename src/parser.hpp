#pragma once

// Put variant into nonstd namespace, 'cause it's f u c k i n g annoying to have
// to type out mapbox::util:: all the time
namespace nonstd {
  using namespace mapbox::util;
}

/** Possible non-terminals */
enum class nterm {
  program,
  statement_or_preprocessor,
  preprocessor,
  statement_list,
  statement,
  expression,
  var_declaration,
  fn_declaration,
  extern_fn_declaration,
  fn_signature,
  fn_type,
  type_declaration,
  type_definition,
  struct_definition,
  struct_field,
  enum_definition,
  enum_field,
  member_var_decl,
  comptime,
  parameter_decl_list,
  template_parameter_decl_list,
  parameter_decl,
  parameter_list,
  template_parameter_list,
  meta_type_ident,
  identifier,
  literal,
  make_expression,
  op,
  if_,
  elif,
  else_,
  lambda,
  for_loop,
  comptime_for_loop,
  comptime_if,
  qualified_name,
  qualified_type,
  binary_expression,
  function_call,
  array_access,
  empty_array_access,
  range,
  assignment,
};

/** A parse node is either a terminal (just a string_view) or a non-terminal. It
    also contains children. */
struct parse_node {
  nonstd::variant<nterm, token> val;
  std::vector<parse_node> children;
  source_label sl;
  bool is_nterm(nterm n) const {
    return val.template is<nterm>() && val.template get<nterm>() == n;
  }
  template <typename T>
  bool is_term_val(T v) const {
    return val.template is<token>() && val.template get<token>().val == v;
  }
  template <typename T>
  bool is_term_type(T v) const {
    return val.template is<token>() && val.template get<token>().type == v;
  }

  inline const token& term() const {
    return val.template get<token>();
  }

  inline std::string to_string() const {
    if (val.template is<nterm>()) {
      std::string res;
      for (const auto& c : children) {
        res += c.to_string() + " ";
      }
      return res;
    } else {
      return std::string(val.template get<token>().val);
    }
  }
};

// Forward declare all the parse functions
parse_node parse_program(lexer& l);
parse_node parse_statement_or_preprocessor(lexer& l);
parse_node parse_preprocessor(lexer& l);
parse_node parse_statement_list(lexer& l);
parse_node parse_statement(lexer& l);
parse_node parse_expression(lexer& l, bool no_right_angle = false);
parse_node parse_var_declaration(lexer& l);
parse_node parse_fn_declaration(lexer& l);
parse_node parse_extern_fn_declaration(lexer& l);
parse_node parse_fn_signature(lexer& l);
parse_node parse_fn_type(lexer& l);
parse_node parse_type_declaration(lexer& l);
parse_node parse_type_definition(lexer& l);
parse_node parse_struct_definition(lexer& l);
parse_node parse_struct_field(lexer& l);
parse_node parse_enum_definition(lexer& l);
parse_node parse_enum_field(lexer& l);
parse_node parse_member_var_decl(lexer& l);
parse_node parse_comptime(lexer& l);
parse_node parse_parameter_decl_list(lexer& l);
parse_node parse_template_parameter_decl_list(lexer& l);
parse_node parse_parameter_decl(lexer& l, bool no_right_angle = false);
parse_node parse_parameter_list(lexer& l);
parse_node parse_template_parameter_list(lexer& l);
parse_node parse_meta_type_ident(lexer& l);
parse_node parse_identifier(lexer& l);
parse_node parse_literal(lexer& l);
parse_node parse_make_expression(lexer& l);
parse_node parse_op(lexer& l);
parse_node parse_for_loop(lexer& l);
parse_node parse_comptime_for_loop(lexer& l);
parse_node parse_if(lexer& l);
parse_node parse_comptime_if(lexer& l);
parse_node parse_elif(lexer& l);
parse_node parse_else(lexer& l);
parse_node parse_lambda(lexer& l);
parse_node parse_qualified_name(lexer& l, parse_node lrec);
parse_node parse_qualified_type(lexer& l, parse_node lrec);
parse_node parse_binary_expression(lexer& l, parse_node lrec);
parse_node parse_function_call(lexer& l, parse_node lrec);
parse_node parse_array_access(lexer& l, parse_node lrec);
parse_node parse_empty_array_access(lexer& l, parse_node lrec);
parse_node parse_range(lexer& l, parse_node lrec);
parse_node parse_assignment(lexer& l, parse_node lrec);

inline parse_node parse(lexer& l) {
  return { parse_program(l) };
}
