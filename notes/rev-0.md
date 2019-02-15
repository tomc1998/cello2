# Cello 2

Second iteration on cello, low-level language focused on compile time metaprogramming

## Required features

### Compile-time code generation

A lisp-esque macro system, with easy splicing.

### Compile-time state and include vs import

Macros can alter global compile-time state, to alter the next time macros run

To make this possible, code execution must be in a fixed order

All top-level static types are parsed ahead of time, and function names &
signatures, in a pre-pass. The modules are processed in a defined order. One
modules compiles the whole project, with that modules including other modules. Each
modules can include any other modules, and a modules cannot be included twice.

All modules will declare the types / functions they declare ahead of time. This way we can
have circular imports (even if includes need to be done in the right order).
This means we can declare functions that take pointers to types which haven't
been finalized, and also forward declare functions in general.

A second pass processes all compile-time code generation / metaprogramming,
and jits all macros. Macros will have access to different levels of
introspection information depending on whether a file has already been
processed or not. Example:

```
We have three files, main.cel, foo.cel, and bar.cel. Foo imports bar, but is included before bar.cel:

/*******************************/
/*          main.cel           */
/*******************************/

#include foo
#include bar


/*******************************/
/*           bar.cel           */
/*******************************/

// A normal struct decl
export type MyStaticStruct = struct {
  a: i32
  b: i32
};

// A struct decl requiring some processing
export type MyDynamicStruct = comptime {
  var struct = meta::struct();
  struct.fields = [
    ("c", i32),
    ("d", i32),
  ];
  struct
};

/*******************************/
/*           foo.cel           */
/*******************************/

#import foo

// Here, we declare an array of ints which is the size of foo::MyStaticStruct
type MyArrayType = comptime {
  var array_type = meta::array();
  var static_struct_type = foo::MyStaticStruct;
  array_type.len = static_struct_type.fields?.len;
  array_type.type = i32;
  array_type
}
var a : MyArrayType; // type i32[2];

// Try and do the same thing with MyDynamicStruct:
type MyArrayType = comptime {
  var array_type = meta::array();
  var dynamic_struct_type = foo::MyStaticStruct;
  // Compile error: dynamic struct type not finalised, so can't access 'fields'.
  array_type.len = dynamic_struct_type.fields?.len;

  ...
}
```

Here, we get a compile error trying to access `fields` on
`dynamic_struct_type`, because fields is not initialised yet (specifically,
it's 'none', see the 'Optional types' feature).

This is because in main.cel we `#include` foo before bar. To resolve this
issue, swap the includes around - this makes bar fully evaluate before foo,
meaning that `MyStaticStruct.fields` is initialised by the time foo runs.

including is NOT the same as importing. A modules can be imported multiple
times, but only included once. 'Inclusion' indicates that a modules is
processed - 'importing' is just a way to import names into the modules.

### Modules

Modules are organised like rust - either a file or folder containing a 'mod.cel'
file is considered a module.

Modules are nested, with mod.cel responsible for #including all the other
modules in that given folder.

#includes can ONLY happen in main.cel, and mod.cel (the main and root-module
files).

ALL module files in the folder must be included - if a file wants to be
ignored, it must be `#ignore`d - e.g. `#ignore foo`. This is to help with
maintaining module ordering.

## Resolving circular templated types

This was an issue in terra (see https://github.com/zdevito/terra/issues/316). We
can template structures with comptime macros:

```
comptime fn Vec = (T: $type) -> $type {
  `struct {
    ptr: *@T,
    len: i32,
    cap: i32,
  }
};

var v0 : Vec(i32) = { ... };
var v1 : Vec(f32) = { ... };
```

However, this becomes annoying if we have a circular type. Consider the following example:
```
type Foo = struct {
  b: &Bar,
};

type Bar = struct {
  f: Foo,
};
```

This won't work, because Foo requires Bar before it's been read.

To fix this, we can do a pre-pass to just parse the names of structs, THEN parse
the structures.

However, this isn't so simple with comptime types, where we can't do this pre-pass:

```
comptime fn Foo = (T: $type) -> $type {
  `struct {
    b: &@Bar(T)
  }
};

comptime fn Bar = (T: $type) -> $type {
  `struct {
    f: @Foo(T),
    val: T
  }
};
```

This results in problems, because in order to construct Foo(T) we need Bar(T),
and in order to construct that we need Foo(T), which hasn't returned yet ...
hence infinite recursion.

Let's see how this would work in C++:

```c++
template <typename T>
struct Bar;

template <typename T>
struct Foo {
  Bar<T>* b;
};

template <typename T>
struct Bar {
  Foo<T> b;
};
```

So, we need a forward declaration, but seems to work okay.

This is a pretty fundamental issue - c++ effectively gives us the ability to
forward declare comptime functions, AND call them without actually having an
implementation, which is hard to do in our more 'general' system.

```

comptime fn Foo = (T: $type) -> $type {
  `struct {
    b: &@Bar(T),
    fn do_stuff() -> @T {
        return b->val;
    }
  }
};

comptime fn Bar = (T: $type) -> $type {
  `struct {
    f: @Foo(T),
    val: T
  }
};
```



## Example syntax


### Importing and including

```
// Identical syntax to rust

// Include the module 'foo', identified by either 'foo.cel' or 'foo/mod.cel'.
#include foo

// Import the module foo. Values exported can be accessed with foo::FOO.
#import foo

// Import everything inside foo into the global namespace
#import foo::*

// import the symbol 'bar' from foo into the global namespace
#import foo::bar

// Import 'bar' and 'baz' from foo
#import foo::{bar, baz}

// More complex example, with nested modules and aliasing
#import foo::{mod1::*, mod2::bar as mod2_bar, bar}
```

### Declaring values

```
// Declare variable - fairly self explanetory
var x = 3;
var x : i32 = 4;

// Declare a type
type Foo = i32;
type Bar = struct {
  a: i32,
  b: i32,
};

// Declare a function
fn my_fn = (a: i32, b: i32) -> Foo {
}
```

### Comptime expressions (0 parameter macros)

```
// Comptime expressions just generate something, like a function or type.
type Foo = comptime {
  i32;
};

fn my_fn = (a: i32, b: i32) -> Foo {
  // Here, we generate the addition expression at compile time, which is then compiled.
  // This is similar to the lisp syntax, where '`' is a 'quote', and '@' can be
  // used to escape (since ',' is used elsewhere, unlike lisp!).
  return comptime {
    `{a + b}
  };
};

fn my_escape_fn = (a: i32) -> Foo {
  // This is an example of splicing in a value at compile time.
  return comptime {
    `{a + @{math::sqrt(2)}}
  };
};

// When quoting / escaping, normally `{} and @{} is used. This can sometimes be omitted, if what you're quoting / escaping is a single expression:
// `foo = valid
// `(foo + bar) = valid
// `if x { foo } else { bar } = valid
// `x + 3 = invalid, this just evaluates to the value of 'x', then + 3. So, if x was 4, this would evaluate to `{3 + 4}, rather than `7.

// In this example, `my_dyn_fn` is only evaluated when this file
// is `#included`, so the function `my_dyn_fn` CANNOT be used until this is fully
// `#included`. In the case of `my_fn`, the signature is declared statically, so
// this CAN be called before being #included.
fn my_dyn_fn = comptime {
  `{
    (a: i32, b: i32) -> Foo {
      a + b
    }
  };
};
```

### Comptime functions (macros)

```
// Function takes a list of expressions, and returns another expression, adding 
them all together. For example, add_list(3, 4, 5) would return (3) + ((4) + (5)). 
add_list(3, a * b, 7 + 8 + 9) would return (3) + ((a * b) + (7 + 8 + 9)).
comptime fn add_list = (args: $expr...) -> $expr {
  if args.len >= 2 {
    `(@args[0] + @add_list(args[1..]))
  } else if args.len == 0 {
    args[0]
  } else {
    `() // Empty expr
  }
};

// Notice, the macro expressions are sanitized by default, by surrounding with ().

// We can use this to template structs. Notice $type instead of $expr - this 
// guarantees that the input is a valid syntax for a type.

comptime fn MyContainer = (T: $type) -> $type {
  `struct {
    val: *@T
  }
};

type I32Container = MyContainer(i32);
type I64Container = MyContainer(i64);

// Again, notice that this file must be evaluated before I32 can be used anywhere.

// NOTE:
// By default, comptime functions are memoized. This means that calling the 
// function twice with the same input will return EXACTLY the same output, by 
// caching the first result and returning it from the second call. This means 
// that two calls to MyContainer(i32) will ACTUALLY return the same type, not just
// a type that has exactly the same fields. This means you can write functions
// that will accept this type:
fn foo = (v: MyContainer(i32)) {
}

// Then call foo with another var of type MyContainer(i32)
var v : MyContainer(i32) = { val: null };
foo(v);

// This will typecheck, as both MyContainer(i32)s are the same.

```
