# MeetixOS C++ Code Design

* In classes public constructors and assignment operator overloads are **not** allowed except for
  * conversion/wrapping constructors (i.e ```Option(T const&)```/```Option(T&&)```)
  * move constructors
  * conversion/wrapping assignment operator overload (i.e ```Option::operator=(T const&)```/```Option::operator=(T&&)```)
  * move assignment operator overload

Instead, static factory functions must be used (i.e ```[try_]construct_<with,from>...()```) to construct objects.

The idea is to promote consciouses operations over the objects i.e, instead of doing:

```c++
String a{ "Hi Guys"sv };
String b{ a }; /* here a copy could occur, but is not that clear */
```

LibTC enforces you to do:

```c++
auto const a = String::from_view("Hi Guys"sv);
auto const b = a.clone();
```