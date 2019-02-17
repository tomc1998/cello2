
parse_node parse_program(lexer& l) {
  parse_error root;
  std::vector<parse_node> children;
  while(l.peek()) {
    try {
      while (l.peek() && l.peek()->val == ";") {
        children.push_back({ *l.next(), {} });
      }
      if (!l.peek()) {
        break;
      }
      children.push_back(parse_statement_or_preprocessor(l));
      if (l.peek() && l.peek()->val == ";") {
        children.push_back({ *l.next(), {} });
      }
    } catch (parse_error e) {
      root.push_back(e);
      if (l.peek()) {
        children.push_back({ *l.next(), {} });
      }
    }
  }
  if (root.children.size() > 0) {
    for (const auto& e : root.children) {
      std::cout << "Error: " << e.sl.to_string(l.whole_file())
                << " - " << e.error << std::endl;
    }
    std::exit(1);
  }
  return { nterm::program, children };
}
