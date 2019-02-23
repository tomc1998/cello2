#pragma once

#include <nonstd/string_view.hpp>
#include <string>
#include <cstdint>

/** A label into the source code */
struct source_label {
  nonstd::string_view file_name;
  /** Byte position */
  std::uint32_t pos;

  source_label(nonstd::string_view file_name,
               std::uint32_t pos)
    : file_name(file_name), pos(pos) {};
  source_label() : file_name(0,0), pos(0) {};

  /** Generate a string from the label & file */
  std::string to_string(nonstd::string_view file) const {
    int line_number = -1, char_number = -1;
    if (file.size() > pos-1) {
      // Count lines
      int lines = 1;
      const char* last_line_start = nullptr;
      for (unsigned ii = 0; ii < pos; ++ii) {
        if (file[ii] == '\n') {
          last_line_start = &*(file.begin() + ii);
          lines += 1;
        }
      }
      line_number = lines;
      if (last_line_start != nullptr) {
        char_number = (int)((file.begin() + pos) - last_line_start - 2);
      }
    }
    if (line_number == -1) {
      return std::string(file_name);
    } else if (char_number == -1) {
      return std::string(file_name) + ":" + std::to_string(line_number);
    } else {
      return std::string(file_name) + ":" + std::to_string(line_number) + ":"
        + std::to_string(char_number);
    }
  }
};
