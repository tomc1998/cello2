
/** Add the builtin types to the given scope */
void add_builtin_types(scope& s, LLVMContext &c) {
  const static type builtin_ty_u64  = { int_type { 64, false }, 0 };
  const static type builtin_ty_u32  = { int_type { 32, false }, 0 };
  const static type builtin_ty_u16  = { int_type { 16, false }, 0 };
  const static type builtin_ty_u8   = { int_type {  8, false }, 0 };
  const static type builtin_ty_i64  = { int_type { 64, true }, 0 };
  const static type builtin_ty_i32  = { int_type { 32, true }, 0 };
  const static type builtin_ty_i16  = { int_type { 16, true }, 0 };
  const static type builtin_ty_i8   = { int_type {  8, true }, 0 };
  const static type builtin_ty_f64  = { float_type { 64 }, 0 };
  const static type builtin_ty_f32  = { float_type { 32 }, 0 };
  const static type builtin_ty_void = { void_type { }, 0 };

  s.symbol_table.insert(std::make_pair("u64",  var { type { kind_type { new type(builtin_ty_u64) },  0 }, (Value*)nullptr, false, false }));
  s.symbol_table.insert(std::make_pair("u32",  var { type { kind_type { new type(builtin_ty_u32) },  0 }, (Value*)nullptr, false, false }));
  s.symbol_table.insert(std::make_pair("u16",  var { type { kind_type { new type(builtin_ty_u16) },  0 }, (Value*)nullptr, false, false }));
  s.symbol_table.insert(std::make_pair("u8",   var { type { kind_type { new type(builtin_ty_u8) },   0 }, (Value*)nullptr, false, false }));
  s.symbol_table.insert(std::make_pair("i64",  var { type { kind_type { new type(builtin_ty_i64) },  0 }, (Value*)nullptr, false, false }));
  s.symbol_table.insert(std::make_pair("i32",  var { type { kind_type { new type(builtin_ty_i32) },  0 }, (Value*)nullptr, false, false }));
  s.symbol_table.insert(std::make_pair("i16",  var { type { kind_type { new type(builtin_ty_i16) },  0 }, (Value*)nullptr, false, false }));
  s.symbol_table.insert(std::make_pair("i8",   var { type { kind_type { new type(builtin_ty_i8) },   0 }, (Value*)nullptr, false, false }));
  s.symbol_table.insert(std::make_pair("f64",  var { type { kind_type { new type(builtin_ty_f64) },  0 }, (Value*)nullptr, false, false }));
  s.symbol_table.insert(std::make_pair("f32",  var { type { kind_type { new type(builtin_ty_f32) },  0 }, (Value*)nullptr, false, false }));
  s.symbol_table.insert(std::make_pair("void", var { type { kind_type { new type(builtin_ty_void) }, 0 }, (Value*)nullptr, false, false }));
}
