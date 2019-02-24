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
