void log_all_parse_errors(const lexer& l, const parse_error& e) {
  if (e.error.size() > 0) {
    std::cout << "Error: " << e.sl.to_string(l.whole_file())
              << " - " << e.error << std::endl;
  }
  for (const auto& c : e.children) {
    log_all_parse_errors(l, c);
  }
}

parse_node parse_program(lexer& l) {
  parse_error root;
  std::vector<parse_node> children;
  while(l.peek()) {
    try {
      while (l.peek() && l.peek()->val == ";") {
        children.push_back({ *l.next(), {}, l.get_curr_source_label() });
      }
      if (!l.peek()) {
        break;
      }
      children.push_back(parse_statement_or_preprocessor(l));
      if (l.peek() && l.peek()->val == ";") {
        children.push_back({ *l.next(), {}, l.get_curr_source_label() });
      }
    } catch (parse_error e) {
      root.push_back(e);
      try_consume_until_after_item(l);
    }
  }
  if (root.children.size() > 0) {
    log_all_parse_errors(l, root);
    std::exit(1);
  }
  return { nterm::program, children, l.get_curr_source_label() };
}
