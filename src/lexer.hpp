std::regex r_ident = std::regex("[A-Za-z_][A-Za-z0-9_]*");
std::regex r_punc = std::regex("(\\{|\\}|\\(|\\)|\\[|\\]|,|\\.|`|@|\\$|&|#|;|:)");
std::regex r_op = std::regex("(\\*|\\+|-|\\/|&&|\\|\\||>=|<=|==|>|<|=|!)");
std::regex r_float_lit = std::regex("[-+]?[0-9]\\.[0-9]");
std::regex r_int_lit = std::regex("(0b|0x)?[0-9]+");
std::regex r_string_lit = std::regex("\"(\\\\.|[^\"\\\\])*\"");
std::regex r_c_string_lit = std::regex("c\"(\\\\.|[^\"\\\\])*\"");
std::regex r_comment = std::regex("(\\/\\/.*\n|\\/\\*.*?\\*\\/)");

inline bool is_whitespace(char c) {
  return c == ' ' || c == '\n' || c == '\r' || c == '\t';
}

enum class token_type {
  float_lit, int_lit, string_lit, c_string_lit, punc, ident, comment, op
};

struct token {
  nonstd::string_view val;
  token_type type;
};

class lexer {
  std::size_t input_ptr = 0;
  nonstd::string_view file_name;
  std::string input;
  nonstd::optional<token> curr_peek;

  void consume_whitespace() {
    while (input_ptr < input.size() && is_whitespace(input[input_ptr])) {
      input_ptr ++;
    }
  }

  nonstd::optional<token> consume() {
    consume_whitespace();
    if (input_ptr >= input.size()) { return nonstd::nullopt; }
    std::cmatch m;
    const char* start = input.c_str() + input_ptr;
    const char* end = input.c_str() + input.size();
    nonstd::optional<token> tok = nonstd::nullopt;
    if (std::regex_search(start, end, m, r_comment, std::regex_constants::match_continuous)) {
      input_ptr += m.length();
      tok = consume();
    } else if (std::regex_search(start, end, m, r_punc, std::regex_constants::match_continuous)) {
      tok = token { nonstd::string_view (start, m.length()), token_type::punc };
      input_ptr += m.length();
    } else if (std::regex_search(start, end, m, r_op, std::regex_constants::match_continuous)) {
      tok = token { nonstd::string_view (start, m.length()), token_type::op };
      input_ptr += m.length();
    } else if (std::regex_search(start, end, m, r_c_string_lit, std::regex_constants::match_continuous)) {
      tok = token { nonstd::string_view (start, m.length()), token_type::c_string_lit };
      input_ptr += m.length();
    } else if (std::regex_search(start, end, m, r_string_lit, std::regex_constants::match_continuous)) {
      tok = token { nonstd::string_view (start, m.length()), token_type::string_lit };
      input_ptr += m.length();
    } else if (std::regex_search(start, end, m, r_float_lit, std::regex_constants::match_continuous)) {
      tok = token { nonstd::string_view (start, m.length()), token_type::float_lit };
      input_ptr += m.length();
    } else if (std::regex_search(start, end, m, r_int_lit, std::regex_constants::match_continuous)) {
      tok = token { nonstd::string_view (start, m.length()), token_type::int_lit };
      input_ptr += m.length();
    } else if (std::regex_search(start, end, m, r_ident, std::regex_constants::match_continuous)) {
      tok = token { nonstd::string_view (start, m.length()), token_type::ident };
      input_ptr += m.length();
    } else {
      throw std::runtime_error(std::string("Failed to match token input: ")
                               + input.substr(input_ptr, std::min(input_ptr + 20, input.size()))
                               + "...");
    }
    return tok;
  }

public:
  lexer(nonstd::string_view file_name, std::string data) : file_name(file_name), input(data) {}


  const nonstd::optional<token> &peek() {
    if (curr_peek) { return curr_peek; }
    curr_peek = consume();
    return curr_peek;
  }

  nonstd::optional<token> next() {
    if (curr_peek) {
      auto res = curr_peek;
      curr_peek = nonstd::nullopt;
      return res;
    } else {
      return consume();
    }
  }

  source_label get_curr_source_label() {
    return { file_name, (unsigned int)input_ptr };
  }

  nonstd::string_view get_remaining_input() {
    if (curr_peek) {
      return { input.c_str() + (input_ptr - curr_peek->val.size()), input.size()
          - (input_ptr - curr_peek->val.size())};
    } else {
      return { input.c_str() + input_ptr, input.size() - input_ptr};
    }
  }
};
