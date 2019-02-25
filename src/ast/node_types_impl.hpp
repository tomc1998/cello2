/** Included after node_types.hpp AND ast_node.hpp, implements node_type
    specific conversion functions */
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
