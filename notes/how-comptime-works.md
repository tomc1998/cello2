# How comptime works

Variables, types, and functions can all individually be marked as comptime.

## Comptime functions

These will be called at compile time, when the runtime function (or file) that
calls it is compiled. These can call any other functions, which are all executed
at compile-time (even non-comptime functions). These can be thought of as
macros, although they can return values.

All parameters of comptime functions must be comptime values.

If a function is not marked as comptime, but all the parameters are comptime values, 

## Comptime types

Values of this type are available at compile time - pretty simple. Things like
literals, or compiletime args, or values formed from other comptime values.

Comptime values can also be used at runtime, but values created by combining
comptime and runtime values will be runtime values.

Comptime types are useless at runtime:

```cello
var x : comptime i32 = 3;
```

This is pointless, since x is a runtime value, so it just restricts
what it can be assigned to:

```cello
var y : i32 = 3;
mut x : comptime i32 = 4;
x = y; // Compile time error, y is not a comptime value
y = x; // This is okay.
```

As such, there is no 'comptime' type modifier - comptime types are largely for
internal use:

```cello
comptime var x : i32 = 3; // This is actually of type comptime i32
comptime var y : comptime i32 = 4; // Compile error, comptime isn't a type specifier

// Here, x is of type comptime i32, and this function can only be called with a comptime value
fn my_function(comptime x: i32) {
  ...
}

my_function(x); // Error, x is not comptime
my_function(y); // Fine
```

Why can non-comptime functions accept comptime values? They effectively act as
template parameters, and can be used for other comptime functions:

```
// Comptime functino
comptime fn foo(comptime x: i32) {...}

// Non-comptime, but with comptime param 
fn bar(comptime x: i32) {
  // This is valid
  foo(x);
}
```

In addition, the 'meta types' (they start with `$`, e.g. `$type`) are
implicitly comptime. Additionally, these cannot be used at runtime, unlike all
other comptime values.

```cello
var x : $type; // Compile error, meta types cannot be used at runtime
comptime var y : $type = Foo; // This is ok
// If you want to get some type data at runtime, just extract them before assigning to a runtime variable.
var foo_name : String = meta::typeof(Foo).name;
```

## Comptime variables

Variables of this type are available at comptime, and can only be assigned to
values of comptime type.

```cello
comptime var x : i32 = 3; // Comptime var
var y : i32 = 4; // Runtime var
comptime var z : i32 = y; // Error, y is runtime variable
```

## Interaction with template variables

Functions can also be templated, which are effectively comptime variables that
can be inferred (so we can avoid typing them).

```cello
// Example of templated function
fn add<T: $type>(a: T, b: T) {
    a + b
}

// Same as add but at comptime. you can think of this as having 3 comptime parameters - the template variable (T), a, and b.
comptime fn comptime_add<T: $type>(comptime a: T, comptime b: T) {
  a + b;
}

// Template variables can just be used as comptime values
comptime fn my_fn<a: i32, b: i32>() {
  comptime_add(a, b);
}

// The only difference is that these can be inferred:

/**
    A mathematical vector
 */
type vector<T: $type, size: i32> = struct {
    data: T[size],
    
    fn add = (other: vector<T, size>) -> vector<T, size> {
        var res : vector<T, size> = undefined;
        comptime for ii in 0..size {
            res.data[ii] = data[ii] + other.data[ii];
        }
        return res;
    }
}

// Type inference shown here:
var my_vec2_0 = make vector { make f32 [ 2.0, 3.0 ] }
var my_vec2_1 = make vector { make f32 [ 4.0, 5.0 ] }
var my_vec2_2 = my_vec2_0.add(my_vec2_1);
// my_vec2_2 is [ 6, 8 ]
```

In the type inference example with vector above, the make expression inferred T
and 'size' from the first parameter, unifying T[size] with f32[2].
