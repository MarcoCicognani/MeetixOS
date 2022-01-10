# MeetiX C++ Coding Style

Everyone who want to contribute to the MeetixOS code should read this document, since each pull-request will be reviewed
using the following guide.

## Control Flows

Here we use:

* Same line open brackets
* Space between keyword and open brace
* Space before and after the condition
* For single line `if`/`for`/`while`/`do while` statement can be avoided brackets, but the code must be at newline

### `If/Else If/Else`

```c++
if ( /* Condition */ ) {
    /* Code */
} else if ( /* Other Condition */ ) {
    /* Other Code */
} else if ( /* Penultimate Condition */ ) {
    /* Penultimate Code */
} else {
    /* Else Code */
}
```

### `For`

```c++
for ( auto i = /* init */; /* Condition */; /* Increment */  ) {
    /* Code */
}

for ( auto i : /* Collections */  ) {
    /* Code */
}
```

### `While`

```c++
while ( /* Condition */ ) {
    /* Code */
}
```

### `Do While`

```c++
do {
    /* Code */
} while ( /* Condition */ );
```

## Naming Convention

Here we use:

* `CamelCase` for `class`/`enum`/`enum class`/`struct`
    * Where acronym must be written in scream case: i.e. the acpi register `class`/`struct` must be
      called `ACPIRegister`
* `snake_case` for variables, functions and methods
* Global variables must be named with `snake_case` but with a prepended `g_`
* Global static variables must be named with `snake_case` but with a prepended `s_`
* Global constants (`const`/`constexpr`) must be named with `SCREAMING_SNAKE_CASE` with a prepended `C_`
* `enum class`'s variants must use `CamelCase`
* `enum`'s variants must use `SCREAMING_SNAKE_CASE` with a prepended `C_`
* `class`/`struct` members must use `snake_case` with a prepended `m_`

```c++
enum class WorkType : u8 {
    Developer,
    Tester,
    SalesPerson 
};

enum Constants {
    C_WORK_FIRST,
    C_WORK_SECOND,
    C_WORK_THIRD
};

class ACPIRegister {
    /* Code */
private:
    Type m_first_value{};
    Type m_second_value{};
};

class SalesPerson {
    /* Code */
};
```