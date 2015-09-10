# The Current State of (Free) Static Analysis


# Jason Turner

 * http://chaiscript.com - *Open Session Thursday (tomorrow) morning at 9:00-9:30*
 * http://cppbestpractices.com
 * http://github.com/lefticus
 * http://cppcast.com
 * @lefticus
 * Independent Contractor - *Always looking for new clients*
 

# Static Analysis

https://en.wikipedia.org/wiki/Static_program_analysis

> Static program analysis is the analysis of computer software that is performed without actually executing programs...

. . .

 * We will be focusing on
    * source code analysis (as opposed to object code)
    * tools that are freely available
    * issues I've seen in the wild

. . .

 * Technically static analysis includes compiler warnings
    * compiler warnings will only be brought up if they are unique to a particular compiler

# Tools

 * cppcheck
 * clang's analyzer
 * msvc++ analyze
 * coverity's 'scan' (free for open source projects)

## Honorable Mention
 
 * metrix++
 * copy-paste detectors


# C++ >= 11 

# Lambdas.1

```cpp
#include <functional>
#include <iostream>

std::function<void ()> do_something()
{
  int some_value = 1;


  return [&some_value](){ std::cout << some_value << '\n'; };
}

int main()
{
  const auto f = do_something();
  f();
}
```

# Lambdas.1

```cpp
#include <functional>
#include <iostream>

std::function<void ()> do_something()
{
  int some_value = 1;

  // some_value won't exist when this function is returned
  return [&some_value](){ std::cout << some_value << '\n'; };
}

int main()
{
  const auto f = do_something();
  f();
}
```

# Lambdas.2

```cpp
#include <functional>
#include <iostream>

std::function<void ()> do_something()
{
  int some_value;


  return [some_value](){ std::cout << some_value << '\n'; };
}

int main()
{
  const auto f = do_something();
  f();
}
```


# Lambdas.2

```cpp
#include <functional>
#include <iostream>

std::function<void ()> do_something()
{
  int some_value;

  // some_value is used uninitialized
  return [some_value](){ std::cout << some_value << '\n'; };
}

int main()
{
  const auto f = do_something();
  f();
}
```

# Lambdas.3

```cpp
#include <functional>
#include <iostream>

std::function<void ()> do_something()
{
  int some_value;


  return [&some_value](){ std::cout << some_value << '\n'; };
}

int main()
{
  const auto f = do_something();
  f();
}
```

# Lambdas.3

```cpp
#include <functional>
#include <iostream>

std::function<void ()> do_something()
{
  int some_value;

  // uninitialized and captured local by reference
  return [&some_value](){ std::cout << some_value << '\n'; };
}

int main()
{
  const auto f = do_something();
  f();
}
```


# Lambdas - Capture Local - Conclusions

 * cppcheck, clang, and coverity got confused as to whether the variable was initialized if captured by reference
 * msvc caught nothing
 
**No tool warned about the actual capture local by reference and return**


# &&

```cpp
#include <utility>


struct Object {
  void do_something() {}
};

void take(Object &&) { }

void do()
{
  Object o;
  take(std::move(o));
  o.do_something();
}

int main()
{
  do();
}
```

# &&

```cpp
#include <utility>


struct Object {
  void do_something() {}
};

void take(Object &&) { }

void do()
{
  Object o;
  take(std::move(o));
  o.do_something(); // use of local after move
}

int main()
{
  do();
}
```

# && - Use After Move - Conclusions

 * No tool commented on this problem at all.

Bonus

 * cppcheck points out that *technically* `Object::do_something` can be static








# General Issues

# assert

```cpp
#include <cassert>

bool do_something(int &i)
{
  ++i;
  return i < 10;
}

int main()
{
  int i = 0;


  assert(do_something(i));

}
```


# assert

```cpp
#include <cassert>

bool do_something(int &i)
{
  ++i;
  return i < 10;
}

int main()
{
  int i = 0;

  // what happens in a release build?
  assert(do_something(i));
  // warning: i is initialized but unused
}
```


# assert

```cpp
#include <cassert>

bool do_something(int &i)
{
  ++i;
  return i < 10;
}

int main()
{
  int i = 0;

  // what happens in a release build?
  assert(do_something(i));
  if (i > 2) { /* ... */ } // no warning
}
```

# assert - Conclusions

No tool tested complained as long as `i` was used.



# branching.1

```cpp
bool test_value_1() { return true; }

bool do_something()
{
  if (test_value_1()) {

    return true;
  } else {

    return true;
  }
}

int main()
{
  do_something();
}
```

# branching.1

```cpp
bool test_value_1() { return true; }

bool do_something()
{
  if (test_value_1()) {

    return true;
  } else {
    // pointless duplication
    return true;
  }
}

int main()
{
  do_something();
}
```

# branching.2

```cpp
bool test_value_1() { return true; }
bool test_value_2() { return true; }

bool do_something()
{
  if (test_value_1()) {

    if (test_value_2()) {
      return true;
    }

    return true;
  }

  return false;
}

int main()
{
  do_something();
}
```


# branching.2

```cpp
bool test_value_1() { return true; }
bool test_value_2() { return true; }

bool do_something()
{
  if (test_value_1()) {
    // this test is useless
    if (test_value_2()) {
      return true;
    }

    return true;
  }

  return false;
}

int main()
{
  do_something();
}
```

# branching - Duplicate Branch - Conclusions

 * Only cppcheck caught either case
 * Surprisingly, this more complicated case was caught by Coverity Scan in ChaiScript

    ```cpp
    if (rt.bare_equal(boxed_type))
    {
      if (lt.bare_equal(boxed_pod_type))
      {
        return true;
      }

      return true;
    }
    ```

    It is unknown why our simplified case was not caught by coverity.




# precision.1

```cpp
// Assume modern 64bit platform
#include <vector>
#include <iostream>

int main()
{
  std::vector<int> v;


  for (unsigned l = 0; l < v.size(); ++l)
  {
    std::cout << v[l] << '\n';
  }
}
```

# precision.1

```cpp
// Assume modern 64bit platform
#include <vector>
#include <iostream>

int main()
{
  std::vector<int> v;

  // this is bad because it limits the loop to 2^32 elements
  for (unsigned l = 0; l < v.size(); ++l)
  {
    std::cout << v[l] << '\n';
  }
}
```

# precision.2

```cpp
// Assume modern 64bit platform
#include <vector>
#include <iostream>

int main()
{
  std::vector<int> v;


  for (long l = 0; l < v.size(); ++l)
  {
    std::cout << v[l] << '\n';
  }
}
```

# precision.2

```cpp
// Assume modern 64bit platform
#include <vector>
#include <iostream>

int main()
{
  std::vector<int> v;

  // this is bad because it limits the loop to 2^63 elements
  for (long l = 0; l < v.size(); ++l)
  {
    std::cout << v[l] << '\n';
  }
}
```

# precision.3

```cpp
// Assume modern 64bit platform
#include <vector>
#include <iostream>

int main()
{
  std::vector<int> v;


  for (int l = 0; l < v.size(); ++l)
  {
    std::cout << v[l] << '\n';
  }
}
```

# precision.3

```cpp
// Assume modern 64bit platform
#include <vector>
#include <iostream>

int main()
{
  std::vector<int> v;

  // this is bad because it limits the loop to 2^31 elements
  for (int l = 0; l < v.size(); ++l)
  {
    std::cout << v[l] << '\n';
  }
}
```

# precision.3

```cpp
// Assume modern 64bit platform
#include <vector>
#include <iostream>

int main()
{
  std::vector<int> v;

  // this is bad because it limits the loop to 2^31 elements
  for (int l = 0; l < v.size(); ++l) // what else is odd?
  {
    std::cout << v[l] << '\n';
  }
}
```


# precision.3

```cpp
// Assume modern 64bit platform
#include <vector>
#include <iostream>

int main()
{
  std::vector<int> v;

  // this is bad because it limits the loop to 2^31 elements
  for (int l = 0; l < v.size(); ++l) // Empty vector
  {
    std::cout << v[l] << '\n';
  }
}
```


# precision - Loss of Sign and Size - Conclusion

 * clang and MSVC warn about sign comparisons
 * No tool catches both

Bonus

 * cppcheck warns us we are iterating over an empty vector


# nullptr.1

```cpp
void do_something()
{
  int *i = nullptr;

  *i = 5;
}

int main()
{
  null_dereference_1();
}
```

# nullptr.1

```cpp
void do_something()
{
  int *i = nullptr;
  // dereferencing obviously null value
  *i = 5;
}

int main()
{
  null_dereference_1();
}
```


# nullptr.2

```cpp
int *get_i() {
  return nullptr;
}

void do_something() 
{
  int *i = get_i();

  *i = 5;
}

int main()
{
  null_dereference_2();
}
```

# nullptr.2

```cpp
int *get_i() {
  return nullptr;
}

void do_something() 
{
  int *i = get_i();
  // Indirect dereference
  *i = 5;
}

int main()
{
  null_dereference_2();
}
```

# nullptr - Null Dereferences - Conclusions

 * Only cppcheck could catch the indirect nullptr dereference


# deadcode

```cpp
#include <system_error>

enum Enum
{
  Value1,
  Value2,
  Value3
};

int go(Enum t_enum) {
  switch (t_enum) {
  case Enum::Value1:
    return 0;
  case Enum::Value2:
    return 1;
  default:
    throw std::runtime_error("unknown value");
  }
  throw std::runtime_error("unknown value"); 
}

int main()
{
  go(Enum::Value3);
}
```

# deadcode

```cpp
#include <system_error>

enum Enum
{
  Value1,
  Value2,
  Value3
};

int go(Enum t_enum) {
  switch (t_enum) {
  case Enum::Value1:
    return 0;
  case Enum::Value2:
    return 1;
  default:
    throw std::runtime_error("unknown value");
  }
  throw std::runtime_error("unknown value");// this code is unreachable
}

int main()
{
  go(Enum::Value3);
}
```

# deadcode - Conclusions

 * MSVC warns about this *in the IDE*
 * clang warns with `-Weverything`

Bonus

 * coverity scan notes that the `throw` is unhandled in `main`


# Conclusion

<table>
<thead>
<tr><th>test</th><th>clang</th><th>cppcheck</th><th>msvc</th><th>coverity scan</th></tr>
</thead>
<tr><td>uninit lambda capture</td><td>X</td><td>X</td><td></td><td>X</td></tr>
<tr><td>uninit lambda capture by ref</td><td></td><td></td><td></td><td></td></tr>
<tr><td>use after move</td><td></td><td></td><td></td><td></td></tr>
<tr><td>assert with side effects</td><td></td><td></td><td></td><td></td></tr>
<tr><td>duplicate else</td><td></td><td></td><td></td><td></td></tr>
<tr><td>duplicate implied else</td><td></td><td>X</td><td></td><td></td></tr>
<tr><td>sign comparison</td><td>X</td><td></td><td>X</td><td></td></tr>
<tr><td>size comparison</td><td></td><td></td><td></td><td></td></tr>
<tr><td>direct nullptr</td><td>X</td><td>X</td><td>X</td><td>X</td></tr>
<tr><td>indirect nullptr</td><td></td><td>X</td><td></td><td></td></tr>
<tr><td>deadcode</td><td>X</td><td></td><td>X</td><td>X</td></tr>
</table>


# Conclusion

> - C++ >= 11 analysis checking still has a long way to go
> - You must use a combination of compilers / analyzers
> - Consider building your own analysis with libclang
>     - http://www.kdab.com/use-static-analysis-improve-performance/
>     - https://github.com/mapbox/cncc
> - Consider adding to https://github.com/lefticus/AnalysisTestSuite


# Jason Turner

 * http://chaiscript.com - *Open Session Thursday (tomorrow) morning at 9:00-9:30*
 * http://cppbestpractices.com
 * http://github.com/lefticus
 * http://cppcast.com
 * @lefticus
 * Independent Contractor - *Always looking for new clients*
 
