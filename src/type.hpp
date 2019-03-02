#pragma once

struct struct_data;
struct scope;
struct type;

/** $type, i.e.
    var a : $type = i32;
    // Here, a is a 'kind_type'.
 */
struct kind_type {
  type* val;
  bool operator==(const kind_type& other) const;
};

struct void_type {
  bool operator==(const void_type& other) const;
};

/** A struct type - the actual data of the fields are stored in struct_data,
    this is so that `type` can remain small & passed around by value since we
    don't own data. */
struct struct_type {
  /** Non-named types typecheck differently - two types are equal if they have
      the same name, OR if neither have the same name and the fields are equal
      (such that they can be pointer cast between one another).
      This name is intrinsic to the type, rather than the variable binding:
      ```c++
      type Foo = struct {};
      type Bar = struct {};
      // Despite being the same memory layout, Foo != Bar.
      type Baz = Bar;
      // Here, even though the variable names are different, Baz == Bar, since
      // they actually the same struct with the same name.
      ```

      This name is not needed to be the same as the variable, and really only
      needs to be a unique identifier. However, it might be useful for it to
      have the same name for debugging / metaprogramming purposes.
  */
  nonstd::optional<nonstd::string_view> name;
  /** A pointer to the struct data. Should never be null! */
  struct_data* data;
  Type* to_llvm_type(const scope& s, LLVMContext &c) const;
  bool operator==(const struct_type& other) const;
};

struct float_type {
  uint16_t num_bits;
  Type* to_llvm_type(const scope& s, LLVMContext &c) const;
  bool operator==(const float_type& other) const;
};

struct int_type {
  uint16_t num_bits;
  bool is_signed;
  Type* to_llvm_type(const scope& s, LLVMContext &c) const;
  bool operator==(const int_type& other) const;
};

struct function_type {
  type* return_type;
  std::vector<type> args;
};

/** A type in the typesystem - feel free to pass this around by value, all the
    types are made to be tiny. */
struct type {
  mapbox::util::variant<int_type, float_type, void_type, struct_type, kind_type> val;
  /** Number of levels of indirection - for int*, this is 1. */
  uint8_t num_ptr;
  Type* to_llvm_type(const scope& s, LLVMContext &c) const;
  /** CLone this type, increment num_ptr */
  type ptr(int levels) const;

  /** Auto-CasCannot coert val to other. Returns nullopt if an implicit cast is not
      possible. */
  nonstd::optional<Value*> coerce(const scope& s, IRBuilder<> &b,
                                  Value* from, const type& target) const;
  std::string to_string() const;
  bool operator==(const type& other) const;
  bool operator!=(const type& other) const;
};

struct struct_field {
  nonstd::string_view name;
  type field_type;
  Type* to_llvm_type(const scope& s, LLVMContext &c) const;
};

/** A method on a struct */
struct struct_method {
  nonstd::string_view name;
  function_type method_type;
  /** The actual LLVM-compiled function */
  llvm::Value* method;
};

/** The container for the actual fields of a struct. Reference fields by index
    so we can push extra fields without invalidating pointers (more important for
    struct methods) */
struct struct_data {
  std::vector<struct_field> fields;
  std::vector<struct_method> methods;
  /** Returns -1 if not found */
  int find_method_index_with_name(nonstd::string_view name) const;
  /** Returns -1 if not found */
  int find_field_index_with_name(nonstd::string_view name) const;
  Type* to_llvm_type(const scope& s, LLVMContext &c) const;
};
