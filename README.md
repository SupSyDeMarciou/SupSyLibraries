# SUPSY LIBRAIRIES
A set of useful functions and types developped for (but independent from) the **SupSyGameEngine**.

## Overview
The SupSyLibrairies (SL) are mainly divided into `maths` and `utils`, and introduce usefuls utilities when dealing with 3D graphics and programming in general. They follow a standardized synthax (for the most part) to provide a smooth experience when using auto-completion and suggestions, but also so they can easily be remembered and infered from previous experience. All functions and most structures have detailled comments that explain how to use them.

Functions which operate on a specific structure or which are tied to a specific process are prefixed by the name of said structure (or a shortened version). Functions which correspond to mathematical operations are suffixed by the name(s) of the structures(s) on which they operate and if they operate on pointers then they are suffixed by `_` or `_s` depending on wether the result is stored in a third structure or in the first operand. 

Some miscellanoeus functions which don't operate on any specific structure are simply prefixed by `SL_`

A structure `my_struct` may either be created on the *stack* using function `my_struct createMyStruct(...)` or simply `my_struct MyStruct(...)` or on the *heap* using function `my_struct* newMyStruct(...)` or (sometimes) simply `my_struct* MyStruct_(...)`.

If the structure is allocated on the *stack* but may have heap allocated fields, it can be properly destroyed using `void destroyMyStruct(my_struct toDestroy)`.
If the structure is allocated on the *heap*, it can be properly destroyed using `void freeMyStruct(my_struct* toFree)`.

Some functions have multiple *overloads* and are therefore suffixed by their specific purpose.

## Maths
The main structures defined in `maths` are:
- `[t]vec[n]`, a vector of `n` elements of type `t` where:
    - `n` is either `2`, `3` or `4`.
    - `t` is either ` ` (float), `d` (double), `i` (int), `li` (long int), `u` (unsigned int), `lu` (long unsigned int) or `b` (bool).
- `quat`, an quaternion of floats.
- `mat`, a matrix of floats of arbitrary size.
- `mat[n]x[n]`, a square matrix of floats of fixed size where `n` is either `2`, `3` or `4`.

The most common operators are:
- `add` (addition), `sub` (subtraction), `mul` (multiplication), `div` (division), `inv` (inversion), `scale` (multiply by scalar), `set` (set to value), `copy` (copy from other)
- `len` (length), `norm` (normalize), `transp` (transpose)
- `min` (componant-wise minimum), `max` (componant-wise maximum), `addS` (addition scaled), `addM` (addition multiplied), `addSM` (addition scaled-multiplied)

There also are a set of functions which convert from a type to another with the synthax `res_struct [startStructName]to[ResStructName](...)` or `res_struct [ResStructName]from[StartStructName](...)`
Sometimes the result structure name used is shortened to be what differs from the starting struct.

## Utils
Utils mainly define general-purpose structures like `array`, `list`, `hashtbl`, `dict` and `arenaAlloc` along with their corresponding constructors, destructors and accessors, as well as struture specific functionnalities.
