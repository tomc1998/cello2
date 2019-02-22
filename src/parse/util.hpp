/**
   Macro utilities for parsing
*/

#define PARSE_ASSERT(cond, label,  msg)         \
  if (!(cond)) {                                \
    throw parse_error((label), (msg));          \
  }

#define PARSE_ASSERT_VAL(l, exp)                                        \
  if (!l.peek()) {                                                      \
    throw parse_error(l.get_curr_source_label(),                        \
                      std::string("Expected '") + exp + "', got EOF");  \
  }                                                                     \
  if (l.peek()->val != (exp)) {                                         \
    throw parse_error(l.get_curr_source_label(),                        \
                      std::string("Expected '") + exp + "', got "       \
                      + std::string(l.peek()->val));                    \
  }

#define PARSE_ASSERT_TYPE(l, exp, human_readable)                 \
  if (!l.peek()) {                                                \
    throw parse_error(l.get_curr_source_label(),                  \
                      std::string("Expected '")                   \
                      + human_readable + "', got EOF");           \
  }                                                               \
  if (l.peek()->type != (exp)) {                                  \
    throw parse_error(l.get_curr_source_label(),                  \
                      std::string("Expected '") + human_readable  \
                      + "', got "                                 \
                      + std::string(l.peek()->val));              \
  }

#define PARSE_ASSERT_NOT_EMPTY(l, msg)                  \
  if (!l.peek()) {                                      \
    throw parse_error(l.get_curr_source_label(), msg);  \
  }


bool is_assignment_op(const token& t) {
  return t.val == "=" || t.val == "+=" || t.val == "-=" || t.val == "/=" || t.val == "*=";
}
