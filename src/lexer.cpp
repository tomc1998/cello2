#include <regex>
#include <nonstd/string_view.hpp>
#include <nonstd/optional.hpp>
#include <mapbox/variant.hpp>
#include "source_label.hpp"
#include "lexer.hpp"

std::regex r_ident = std::regex("[A-Za-z_][A-Za-z0-9_]*");
std::regex r_punc = std::regex("(\\{|\\}|\\(|\\)|\\[|\\]|,|\\.\\.|\\.|`|@|\\$|#|;|::|:)");
std::regex r_op = std::regex("(->|!=|%=|\\+=|-=|\\*=|\\/=|&=|\\|=|&&|\\|\\||\\||&|>=|<=|==|>|<|=|\\*|\\+|-|\\/|%|!|&)");
std::regex r_float_lit = std::regex("[-+]?[0-9]\\.[0-9]");
std::regex r_int_lit = std::regex("(0b|0x)?[0-9]+");
std::regex r_string_lit = std::regex("\"(\\\\.|[^\"\\\\])*\"");
std::regex r_c_string_lit = std::regex("c\"(\\\\.|[^\"\\\\])*\"");
std::regex r_comment = std::regex("(\\/\\/.*\n|\\/\\*.*?\\*\\/)");

inline bool is_whitespace(char c) {
  return c == ' ' || c == '\n' || c == '\r' || c == '\t';
}

void lexer::consume_whitespace() {
  while (input_ptr < input.size() && is_whitespace(input[input_ptr])) {
    input_ptr ++;
  }
}

nonstd::optional<token> lexer::consume() {
  consume_whitespace();
  if (input_ptr >= input.size()) { return nonstd::nullopt; }
  std::cmatch m;
  const char* start = input.c_str() + input_ptr;
  const char* end = input.c_str() + input.size();
  nonstd::optional<token> tok = nonstd::nullopt;
  if (std::regex_search(start, end, m, r_comment, std::regex_constants::match_continuous)) {
    input_ptr += m.length();
    tok = consume();
  } else if (std::regex_search(start, end, m, r_op, std::regex_constants::match_continuous)) {
    tok = token { nonstd::string_view (start, m.length()), token_type::op };
    input_ptr += m.length();
  } else if (std::regex_search(start, end, m, r_punc, std::regex_constants::match_continuous)) {
    tok = token { nonstd::string_view (start, m.length()), token_type::punc };
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
  } else if (std::regex_search(start, end, m, r_comment, std::regex_constants::match_continuous)) {
    input_ptr += m.length();
    tok = consume();
  } else {
    throw std::runtime_error(std::string("Failed to match token input: ")
                             + input.substr(input_ptr, std::min(input_ptr + 20, input.size()))
                             + "...");
  }
  return tok;
}

lexer::lexer(nonstd::string_view file_name, std::string data) : file_name(file_name), input(data) {}

bool strip_comments = true;

const token *lexer::peek(int n) {
  assert(n < MAX_PEEK);
  while (num_peeks <= n) {
    const auto consumed = consume();
    if (!consumed) {
      return nullptr;
    }
    peek_storage[num_peeks++] = *consumed;
  }
  return &peek_storage[n];
}

const token *lexer::peek() {
  return peek(0);
}

nonstd::optional<token> lexer::next() {
  if (num_peeks > 0) {
    auto res = peek_storage[0];
    for (int ii = 0; ii < num_peeks - 1; ++ii) {
      peek_storage[ii] = peek_storage[ii+1];
    }
    num_peeks --;
    return res;
  } else {
    return consume();
  }
}

source_label lexer::get_curr_source_label() {
  return { file_name, (unsigned int)input_ptr };
}

std::string lexer::get_remaining_input() {
  std::string res = "";
  for (int ii = 0; ii < num_peeks; ++ii) {
    res += std::string(peek_storage[ii].val) + " ";
  }
  res += &*(input.begin() + input_ptr);
  return res.substr(0, 20);
}

void lexer::backup() {
  assert(!backup_storage && "Can't backup, backup already present");
  backup_storage = serialized_lexer { input_ptr, num_peeks, {} };
  std::memcpy(backup_storage->peek_storage, peek_storage, sizeof(peek_storage));
}

void lexer::clear_backup() {
  backup_storage = nonstd::nullopt;
}

void lexer::restore() {
  assert(backup_storage && "Can't restore, backup not present");
  input_ptr = backup_storage->input_ptr;
  num_peeks = backup_storage->num_peeks;
  std::memcpy(peek_storage, backup_storage->peek_storage, sizeof(peek_storage));
  clear_backup();
}

nonstd::string_view lexer::whole_file() const {
  return nonstd::string_view(input);
}
