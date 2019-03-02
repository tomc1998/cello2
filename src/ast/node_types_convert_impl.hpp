/** Included after node_types.hpp AND ast_node.hpp, implements node_type
    specific conversion fns */

/**
   @param n - Either an nterm::program or nterm::statement_list
 */
statement_list::statement_list(const parse_node& n) {
  if (n.is_nterm(nterm::program)) {
    for (const auto c : n.children) {
      if (c.is_nterm(nterm::statement_or_preprocessor)) {
        const auto& s = c.children[0];
        if (s.is_nterm(nterm::preprocessor)) {
          std::cerr << "Preprocessor unsupported" << std::endl;
        }
        children.push_back(ast_node(s));
      }
    }
  } else if (n.is_nterm(nterm::statement_list)) {
    for (const auto c : n.children) {
      if (c.is_nterm(nterm::statement)) {
        children.push_back(ast_node(c));
      }
    }
  } else {
    assert(false);
  }
  is_last_item_expr = n.children[n.children.size()-2].is_term_val(";");
}

/**
   @param n - An nterm::parameter_decl
 */
fn_arg::fn_arg(const parse_node &n) {
  if (n.children.size() == 1) {
    // This doesn't contain a name
    name = nonstd::nullopt;
    type = ast_node(n.children[0]);
  } else {
    name = n.children[0].children[0].term().val;
    type = ast_node(n.children[2]);
  }
}

/**
   @param n - An nterm::fn_declaration
*/
fn_declaration::fn_declaration(const parse_node &n) {
  // Check this is a fn declaration (rather than an extern decl)
  body = nullptr;
  if (n.is_nterm(nterm::fn_declaration)) {
    int curr_ix = 0;
    if (n.children[curr_ix].is_term_val("export")) {
      is_export = true;
      curr_ix ++;
    } else {
      is_export = false;
    }
    curr_ix ++; // fn
    name = n.children[curr_ix++].children[0].term().val;
    if (n.children[curr_ix].is_nterm(nterm::template_parameter_decl_list)) {
      int curr_tp_ix = 0;
      while (!n.children[curr_ix].children[curr_tp_ix].is_term_val(">")) {
        if (n.children[curr_ix].children[curr_tp_ix].is_nterm(nterm::parameter_decl)) {
          template_arg_list.push_back(fn_arg(n.children[curr_ix].children[curr_tp_ix]));
        }
        curr_tp_ix += 1;
      }
      curr_ix += 1;
    }
    curr_ix ++; // =
    // Now we're parsing a fn signature
    const auto &fn_sig = n.children[curr_ix++];
    const auto param_decl_list = fn_sig.children[0];
    // Parse args
    unsigned curr_pd_ix = 0;
    while (!param_decl_list.children[curr_pd_ix].is_term_val(")")) {
      if (param_decl_list.children[curr_pd_ix].is_nterm(nterm::parameter_decl)) {
        arg_list.push_back(fn_arg(param_decl_list.children[curr_pd_ix]));
      }
      curr_pd_ix += 1;
    }
    // Is mut?
    unsigned curr_fn_sig_ix = 1;
    if (fn_sig.children.size() > curr_fn_sig_ix
        && fn_sig.children[curr_fn_sig_ix].is_term_val("mut")) {
      curr_fn_sig_ix ++;
      is_mut = true;
    }
    if (fn_sig.children.size() > curr_fn_sig_ix
        && fn_sig.children[curr_fn_sig_ix].is_term_val("->")) {
      return_type = new ast_node(fn_sig.children[curr_fn_sig_ix + 1]);
    }
    // Now we're parsing the body
    body = new ast_node(n.children[curr_ix++]);
  } else if (n.is_nterm(nterm::extern_fn_declaration)) {
    assert(false && "Unsupported converting extern_fn_declaration to ast");
  }
}

ast_node::ast_node(const parse_node& n)
  : sl(n.sl) {
  init(n);
}

void ast_node::init(const parse_node& n) {
  if (n.is_nterm(nterm::program) || n.is_nterm(nterm::statement_list)) {
    val = statement_list(n);
  } else if (n.is_nterm(nterm::statement)) {
    if (n.children[0].is_nterm(nterm::expression)) {
      init(n.children[0]);
    } else if (n.children[0].is_nterm(nterm::fn_declaration)
               || n.children[0].is_nterm(nterm::extern_fn_declaration)) {
      val = fn_declaration(n.children[0]);
    } else {
      std::cerr << "Statement type not supported" << std::endl;
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
