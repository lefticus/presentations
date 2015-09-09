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

> Static program analysis is the analysis of computer software that is performed without actually executing programs (analysis performed on executing programs is known as dynamic analysis).[1] In most cases the analysis is performed on some version of the source code, and in the other cases, some form of the object code.

. . .

 * We will be focusing on
    * source code analysis
    * tools that are freely available

. . .

 * Technically this includes compiler warnings
    * compiler warnings will only be brought up if they are unique to a particular compiler

# Tools

 * cppcheck
 * clang's analyzer
 * msvc++ analyze
 * coverity's 'scan' (free for open source projects)


## Honorable Mention
 
 * metrix++
 * copy-paste detectors


# Case 1

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

# Case 1

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

# Case 1

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

# Case 1: Assert Conclusions

No tool tested complained as long as `i` was used.


# Case 2.1

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

# Case 2.1

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

# Case 2.2

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


# Case 2.2

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

# Case 2.3

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

# Case 2.3

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


# Case 2: Capture Local Conclusions

 * cppcheck was able to warn on all uninitialized use cases
 * clang and coverity got confused as to whether the variable was initialized if captured by reference
 * msvc caught nothing
 
**No tool warned about the actual capture local by reference and return**


# Case 3.1

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

# Case 3.1

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

# Case 3.2

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


# Case 3.2

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

# Case 3: Duplicate Branch Conclusions

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




# Case 4.1

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

# Case 4.1

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

# Case 4.2

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

# Case 4.2

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

# Case 4.3

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

# Case 4.3

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

# Case 4.3

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


# Case 4.3

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


# Case 4: Loss of Sign and Precision

 * clang and MSVC warn about sign comparisons
 * No tool catches both

Bonus

 * cppcheck warns us we are iterating over an empty vector


# Case 5.1

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

# Case 5.1

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


# Case 5.2

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

# Case 5.2

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

# Case 5 - Null Dereferences

 * Only cppcheck could catch the indirect nullptr dereference

# Case 6

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

# Case 6

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

# Case 6 - Unreachable Code

 * MSVC warns about this *in the IDE*
 * clang warns with `-Weverything`

# Case 7

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

# Case 7

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

# Case 7 - Use After Move

 * No tool commented on this probable at all.

Bonus Note:

 * cppcheck points out that *technically* `Object::do_something` can be static




