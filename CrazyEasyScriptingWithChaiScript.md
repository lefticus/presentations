# Crazy Easy Scripting with ChaiScript



# Jason Turner

 * http://chaiscript.com
 * http://cppbestpractices.com
 * http://github.com/lefticus
 * http://cppcast.com
 * @lefticus
 * Independent Contractor - *Always looking for new clients*


# ChaiScript

 - Header only
 - 0 external dependencies
 - Mature: 6 years old
 - Stable: every release is tested against 
    - clang / MSVC / g++ 
    - MacOS / Windows / Linux
    - 64bit / 32bit
    - AddressSanitizer / ThreadSanitizer
    - MSVC's Static Analyzer / cppcheck
 - Thread safe by default
 - Crazy easy to use


# ChaiScript History

> - Developed with SWIG but the build process overhead was frustrating for small projects
> - At the same time I started wondering what it would take to do multimethod dispatch with C++
> - And my cousin, a language geek, was interested in working on a language with me.

. . . 

Then ChaiScript was born. First as a toolkit to build your own scripting language with ChaiScript being just one reference implementation.

. . .

You can still see some of this in the file layout with the `dispatchkit` and `language` sub folders.

# ChaiScript Goals

> - Syntax feel natural to C++ developers
> - Seemless integration with C++
> - Not get in the way
> - No pre-processor or complex build process
> - Be "fast enough"

# Basics

```cpp
#include <chaiscript/chaiscript.hpp>

int main()
{
  chaiscript::ChaiScript chai;

  chai.eval(R"(
    print("Hello ChaiScript")
  )");
}
```

# Adding a Function

```cpp
#include <chaiscript/chaiscript.hpp>
#include <iostream>

void say_hi()
{
  std::cout << "Hello C++\n";
}

int main()
{
  chaiscript::ChaiScript chai;

  chai.add(chaiscript::fun(&say_hi), "say_hi");

  chai.eval(R"(
    say_hi();
  )");
}
```

# Sharing a Value

```cpp
#include <chaiscript/chaiscript.hpp>

int main()
{
  chaiscript::ChaiScript chai;

  int &i = chai.eval<int &>(R"(
    var i = 5;
    i;
  )");

  i = 20;

  chai.eval(R"(
    print(i); // prints 20
  )"
}
```



# Jason Turner

 * http://chaiscript.com
 * http://cppbestpractices.com
 * http://github.com/lefticus
 * http://cppcast.com
 * @lefticus
 * Independent Contractor - *Always looking for new clients*



