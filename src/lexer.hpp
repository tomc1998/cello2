
constexpr int MAX_PEEK = 2;

enum class token_type {
  float_lit, int_lit, string_lit, c_string_lit, punc, ident, comment, op
};

struct token {
  nonstd::string_view val;
  token_type type;
};

// For backing up
struct serialized_lexer {
  std::size_t input_ptr;
  int num_peeks;
  token peek_storage[MAX_PEEK];
};

class lexer {
  std::size_t input_ptr = 0;
  nonstd::string_view file_name;
  std::string input;

  /** The amount of peeks we have stored in peek_storage */
  int num_peeks = 0;
  token peek_storage[MAX_PEEK];

  nonstd::optional<serialized_lexer> backup_storage;

  void consume_whitespace();

  nonstd::optional<token> consume();

public:
  lexer(nonstd::string_view file_name, std::string data);

  bool strip_comments = true;

  const token *peek(int n);

  const token *peek();

  nonstd::optional<token> next();

  source_label get_curr_source_label();

  std::string get_remaining_input();

  void backup();

  void clear_backup();

  void restore();

  nonstd::string_view whole_file() const;
};
