# MeetixOS C++ Code Design

* In classes public constructors and assignment operator overloads are **not** allowed except for
  * ```noexcept``` conversion/wrapping constructors (i.e ```Option(T const&)```/```Option(T&&)```)
  * ```noexcept``` move constructors
  * ```noexcept``` conversion/wrapping assignment operator overload (i.e ```Option::operator=(T const&)```/```Option::operator=(T&&)```)
  * ```noexcept``` move assignment operator overload

Instead, static factory functions must be used (i.e ```[try_]construct_<with,from>...()```) to construct objects.

The idea is to promote consciouses operations over the objects i.e, instead of doing:

```c++
String a{ "Hi Guys"sv };
String b{ a }; /* here a copy could occur, but is not that clear */
```

LibTC enforces you to do:

```c++
auto const a = String::construct_from_view("Hi Guys"sv);
auto const b = a.clone();
```