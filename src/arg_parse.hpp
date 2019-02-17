/* A declaration of all flags that take arguments */
const char* flags_with_arguments[] = {"f"};

struct prog_arg_parse_exception {
  std::string error_msg;
};

struct prog_arg {
  nonstd::string_view name;
  nonstd::optional<nonstd::string_view> val;
  bool is_flag;
  prog_arg(nonstd::string_view name,
           nonstd::optional<nonstd::string_view> val,
           bool is_flag)
    : name(name), val(val), is_flag(is_flag) {};
};

/** A wrapper around a vector of args, for querying the args */
struct prog_args {
  std::vector<prog_arg> arg_list;

  /** Returns nullptr if the flag doesn't exist. */
  const prog_arg* find_flag(nonstd::string_view flag_name) const {
    const auto res = std::find_if(arg_list.begin(), arg_list.end(),
                                  [&](const auto &a) { return a.name == flag_name; });
    return (res == arg_list.end()) ? nullptr : &*res;
  }

  int num_positional() const {
    return std::count_if(arg_list.begin(), arg_list.end(),
                         [](const auto &a) { return !a.is_flag; });
  }

  const prog_arg& get_positional(int pos) const {
    assert(pos < num_positional());
    for (unsigned ii = 0; ii < arg_list.size(); ++ii) {
      if (!arg_list[ii].is_flag) {
        if (--pos < 0) {
          return arg_list[ii];
        }
      }
    }
    assert(false);
  }
};

/** Throws an arg_parse_exception containing an error msg */
prog_args parse_prog_arg_list(int argc, const char** argv);

bool does_flag_have_arg(nonstd::string_view flag) {
  int size = sizeof(flags_with_arguments)/sizeof(flags_with_arguments[0]);
  for (int ii = 0; ii < size; ++ii) {
    if (flags_with_arguments[ii] == flag) {
      return true;
    }
  }
  return false;
}

prog_args parse_prog_arg_list(int argc, const char** argv) {
  std::vector<prog_arg> arg_list;
  prog_arg* last_flag = nullptr;
  arg_list.reserve(argc); // Rough guess (upper bound)
  // First arg is the program name, skip
  for (int ii = 1; ii < argc; ++ii) {
    if (argv[ii][0] == '-') {
      if (argv[ii][1] == '-') {
        // Long flag
        assert(false && "Unimplemented");
      } else if (argv[ii][1] != '\0') {
        // Short flag[s]. Add them all.
        for (int jj = 1; argv[ii][jj] != '\0'; ++jj) {
          arg_list.emplace_back(nonstd::string_view(argv[ii] + jj, 1), nonstd::nullopt, true);
          if (does_flag_have_arg(nonstd::string_view(argv[ii] + jj, 1))) {
            last_flag = arg_list.data() + arg_list.size() - 1;
          }
        }
      } else {
        throw prog_arg_parse_exception { "Found '-' with no attached flag" };
      }
    } else {
      // Positional arg
      nonstd::string_view val(argv[ii]);
      assert(val.size() != 0 && "Empty arg??");
      if (last_flag) {
        last_flag->val = { val };
        last_flag = nullptr;
      } else {
        arg_list.emplace_back(val, nonstd::nullopt, false);
      }
    }
  }
  return { arg_list };
}
