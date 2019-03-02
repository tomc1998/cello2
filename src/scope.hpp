#pragma once

#include "type.hpp"
#include <nonstd/string_view.hpp>
#include <llvm/IR/Value.h>
#include <llvm/IR/Type.h>
#include <map>
#include <nonstd/optional.hpp>
#include <mapbox/variant.hpp>

struct scope;

/** Used for selecting a symbol with a given type */
enum class named_value_type { type, var, function };

struct var {
  /** The type of this variable */
  type var_type;
  /** This will always be an alloca, and will need to be loaded, assuming this
      is a 'classical' variable - if this var is a function_type, it'll just be the
      function value, and can be called immediately.

      TODO Figure out semantics for lambdas here
  */
  Value* val;

  bool is_mutable : 1;
  bool is_member : 1;
};

struct scope {
  scope* parent = nullptr;
  /** The current context's 'this' for methods, if we're currently in a method */
  nonstd::optional<var> this_ptr = nonstd::nullopt;
  std::map<nonstd::string_view, var> symbol_table;

  scope create_subscope() {
    return { this, {}, {} };
  }

  /** Nullptr if not found */
  const var* find_symbol(const nonstd::string_view name) const {
    const auto res = symbol_table.find(name);
    if (res == symbol_table.end()) {
      if (parent) {
        return parent->find_symbol(name);
      } else {
        return nullptr;
      }
    } else {
      return &res->second;
    }
  }

  /** Nullptr if not found */
  var* find_symbol(const nonstd::string_view name) {
    const auto res = symbol_table.find(name);
    if (res == symbol_table.end()) {
      if (parent) {
        return parent->find_symbol(name);
      } else {
        return nullptr;
      }
    } else {
      return &res->second;
    }
  }

  /** nullptr if not in a method */
  const var* get_this_ptr() const {
    if (this_ptr) { return &*this_ptr; }
    else if (parent) { return parent->get_this_ptr(); }
    else { return nullptr; }
  }
};
