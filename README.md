# SUPSY LIBRAIRIES
## Description
The SupSyLibrairies (SL) are a set of useful functions and types developped for (but independent from) the **SupSyGameEngine**. They are mainly divided into a **maths** and a **utils** section, introducing usefuls utilities when dealing with 3D graphics but also for programming in general. 
Functions and structures introduced follow a standardized synthax (for the most part) to provide a smooth experience when using auto-completion and suggestions, but also so they can easily be remembered and infered from previous experience. All functions and most structures have detailled comments which explain how to use them.

## Prerequisites
This librairy is standalone and ready to be used as-is. It provides some additionnal features when using [**GLFW3**](https://www.glfw.org/) but it is not required for the majority of the SL.

## Synthax Overview
### Structures
Structures don't follow any specific naming convention other than just being as explicit as possible without becoming too anoying to type. However, functions which are tied to specific structures (which work as class methods) have naming rules which are detailed later.

### Operators
Functions which correspond to mathematical operations are suffixed by the name(s) of the structures(s) on which they operate and if they operate on pointers then they are suffixed by `_` or `_s` depending on wether the result is stored in a third structure or in the first operand.

>For example: <br>
>`vec3 add3(vec3 a, vec3 b);` is an *add* operation between two `vec3` which reduces to just `3`. <br>
>`vec3* add3_(vec3* a, vec3* b, vec3* c);` is an *add* operation between two `vec3` but is assumed to work on the heap. Therfore an output pointer *c* is given and the fuction has a `_` suffix. <br>
>`vec3* add3_s(vec3* a, vec3* b);` is an *add* operation between two `vec3` but is assumed to store its result back into *a*. Therfore the function has a `_s` suffix. <br>
>`vec2* mul2x2_2_s(mat2x2* m, vec2* v);` is an *mul* (multiply) operation between a `mat2x2` (reduced to `2x2`) and a `vec2` (reduced to `2`) assumed to store its result back into *v*. Therfore, the function also has a `_s` suffix.

Some miscellanoeus functions are simply prefixed by `SL_`. These usually either work on builtin' C types or are just not tied to a specific structure.

### Functions
Functions which operate on a specific structure or which are tied to a specific process are prefixed by the name of said structure or process (or a shortened version).
>For example: <br>
>`void* dictGet(dict* d, void* key);` operates on structure `dict` and *gets* the element associated with a certain *key*. <br>
>`void debugStartTimer(timer ID);` operates on the process `debug` and *starts* the *timer* of given *ID*.

In addition, most structures have **constructors** and **destructors** for *stack* allocation as well as *heap* allocation which follow this synthax: <br>
> Let's say we are working with this struct: `typedef struct MyStruct my_struct;`. Then: <br>
> - `my_struct createMyStruct(...);` and (sometimes) `my_struct MyStruct(...);` allocate an instance of `my_struct` on the *stack*.
> - `my_struct* newMyStruct(...);` and (sometimes) `my_struct* MyStruct_(...);` allocate an instance of `my_struct` on the *heap*.
> - `void destroyMyStruct(my_struct toDestroy);` frees the fields of `toDestroy` (defined only for structures which allocate some of their fields on the heap).
> - `void freeMyStruct(my_struct* toFree);` frees `toFree`.

Some functions have multiple *overloads* and are therefore suffixed with their specific purpose:
> `quat Quat(float x, float y, float z, float w)` allocates a quaternion `quat` on the *stack* <br>
> `quat Quat_Euler(float l, float m, float n)` allocates a **unit** quaternion `quat` on the *stack* **representing the rotation given by the Euler angles l, m and n**. <br>

## Content Overview
### Maths
The mathematical structures defined by the SL are:
>- `[t]vec[n]`, a vector of `n` elements of type `t` where:
>    - `n` is either `2`, `3` or `4`.
>    - `t` is either ` ` (float), `d` (double), `i` (int), `li` (long int), `u` (unsigned int), `lu` (long unsigned int) or `b` (bool).
>- `quat`, an quaternion of floats.
>- `mat`, a matrix of floats of arbitrary size.
>- `mat[n]x[n]`, a square matrix of floats of fixed size where `n` is either `2`, `3` or `4`.

Arithmetic operators such as addition, subtraction, multiplication, inversion, scaling and many others are supported by all types for which they have a mathematical meaning (specificaly with regards to linear algebra).
Some operators act on multiple structures at once to allow for more flexibility.
Most mathematical structures can be converted into any other when the convertion makes sense (for exemple a rotation matrix to a quaternion, a float vector to a cross-product matrix or any vector to any other of identical size).

### Utils
Utils mainly define general-purpose structures such as `array`, `list`, `hashtbl`, `dict` and `arenaAlloc`, alongside their corresponding constructors, destructors and accessors, as well as struture specific functionnalities.
Some of these structures have associated *macros* to make the user experience a bit more pleasant, but most of them are just wrappers around a function of identical name but with suffix `_P` (for "proper"). This makes it easier to interface with these functions as they usually work with `void*` pointers to make their functionnality independant of types.

Other functions include thoses defined in `inout.h` which are mainly prefixed by `SL_` and which operate on strings, either by analysing them of by printing to a certain output.

In addition, when using **GLFW3**, basic timers for profiling are defined in `debug.h`.
