# Thinking Portable

*Why and How to make your C++ Cross Platform*

# Jason Turner

 * http://chaiscript.com
 * http://cppbestpractices.com
 * http://github.com/lefticus
 * http://cppcast.com
 * @lefticus
 * Independent Contractor

# ChaiScript

```cpp
int dosomething(int x, int y,
                const std::function<int (int, int)> &f)
{ return f(x*2, 3); }

int main()
{
  using namespace chaiscript;

  ChaiScript chai(chaiscript::Std_Lib::library());
  chai.add(fun(&dosomething), "dosomething");
  auto i = chai.eval<int>("dosomething(4,3, `+`)"); // i = 11
}
```

# All My C++ Has Been Cross-Platform

 * Linux i386
 * Linux x86_64
 * Linux MIPS
 * Linux ARM
 * Win32 gcc/msvc
 * Win64 gcc/msvc
 * MacOS i386
 * MacOS x86_64
 * Solaris Sparc
 * FreeBSD
 * Haiku


# Regular Cross-platform Releases

 * Last 5 years spent contracting with a team making regular (bi-weekly) releases of desktop applications
 * MacOS / Linux / Windows
 * Must be easy to install, usable and feel natural on native OS
 * Also deploys ruby bindings for C++ libraries on all OSes

# Goals

 * I’m here to convince you to make all of your C++ applications cross platform  
 * Help you convince your co-workers
 * Give some practical advice on how

# Cross Platform Code is Better!

 * More standards compliant
 * Safer
 * Future resistant
 * More organized
 * More tools available
 * Wider customer base

# Cross Platform Code is Better!

 * More standards compliant
 * Safer
 * Future resistant

These three come with using compilers from multiple vendors on multiple platforms


# Pick *At Least* Two From Each Column

|OS               | Compiler |
|-----------------|----------|
|Windows          |          |
|Linux            | Clang    |
|Android          | MSVC     |
|iOS              | GCC      |
|MacOS            | Intel    |
|Other            |          |



# More Standards Compliant

```cpp
#include <Windows.h>
#include <iostream>

int main()
{
  BOOL b = true;

  if (b) {
    std::cout << "true";
    ++b;

  }
}
```

# More Standards Compliant

```cpp
// #include <Windows.h>
#include <iostream>

int main()
{
  /* BOOL */ bool b = true;

  if (b) {
    std::cout << "true";
    ++b;

  }
}
```

# More Standards Compliant

```cpp
// #include <Windows.h>
#include <iostream>

int main()
{
  /* BOOL */ bool b = true;

  if (b) {
    std::cout << "true";
    ++b; // 9 : warning: incrementing expression of type bool is deprecated 
         // [-Wdeprecated-increment-bool] (-Weverything on clang)
  }
}
```

# More Standards Compliant

 * Only clang on non-Windows would find all of the portability and semantic issues in this code.

# Safer

```cpp
if (Hex_()) {
  std::string match(start, m_input_pos);
  m_match_stack.emplace_back(
    make_node<eval::Int_AST_Node>(
      std::move(match),
      prev_line,
      prev_col,
      buildInt(std::hex, match)));
  return true;
}
```

# Safer 

```cpp
if (Hex_()) {
  std::string match(start, m_input_pos);
  m_match_stack.emplace_back(
    make_node<eval::Int_AST_Node>(
      std::move(match), // move.. 
      prev_line,
      prev_col,
      buildInt(std::hex, match))); // then use?
  return true;
}
```


# Safer 

```cpp
if (Hex_()) {
  std::string match(start, m_input_pos);
  m_match_stack.emplace_back(
    make_node<eval::Int_AST_Node>(
      std::move(match), // move.. 
      prev_line,
      prev_col,
      buildInt(std::hex, match))); // then use?
  return true;
}
```

 * Clang on Linux crashes, no other tool even generated a warning.



# Safer

http://googleresearch.blogspot.no/2006/06/extra-extra-read-all-about-it-nearly.html

```cpp
uint64_t binarySearch(const std::vector <int64_t> &v, int64_t key) {
  int low = 0;
  int high = v.size() - 1;


  while (low <= high) {
    int mid = (low + high) / 2;
    int midVal = v[mid];

    if (midVal < key)
      low = mid + 1;
    else if (midVal > key)
      high = mid - 1;
    else
      return mid; // key found
  }
  return -(low + 1); // key not found.
}
```


# Safer

http://googleresearch.blogspot.no/2006/06/extra-extra-read-all-about-it-nearly.html

```cpp
uint64_t binarySearch(const std::vector <int64_t> &v, int64_t key) {
  int low = 0;
  int high = v.size() - 1;       // No warnings on GCC at any level


  while (low <= high) {
    int mid = (low + high) / 2;  
    int midVal = v[mid];         // What happens with > 2B objects?

    if (midVal < key)
      low = mid + 1;
    else if (midVal > key)
      high = mid - 1;
    else
      return mid; // key found
  }
  return -(low + 1); // key not found.
}
```


# Safer

http://googleresearch.blogspot.no/2006/06/extra-extra-read-all-about-it-nearly.html

```cpp
uint64_t binarySearch(const std::vector <int64_t> &v, int64_t key) {
  int low = 0;
  int high = v.size() - 1; // !!warning: implicit conversion loses integer precision: 
                           // 'unsigned long' to 'int' 
                           // [-Wshorten-64-to-32] (clang -Weverything)
  while (low <= high) {
    int mid = (low + high) / 2;
    int midVal = v[mid];

    if (midVal < key)
      low = mid + 1;
    else if (midVal > key)
      high = mid - 1;
    else
      return mid; // key found
  }
  return -(low + 1); // key not found.
}
```

# Safer

http://googleresearch.blogspot.no/2006/06/extra-extra-read-all-about-it-nearly.html

```cpp
uint64_t binarySearch(const std::vector <int64_t> &v, int64_t key) {
  int low = 0;
  int high = v.size() - 1; // warning C4267: 'initializing': conversion from 'size_t' 
                           //to 'int', possible loss of data (MSVC  /W3)

  while (low <= high) {
    int mid = (low + high) / 2;
    int midVal = v[mid];

    if (midVal < key)
      low = mid + 1;
    else if (midVal > key)
      high = mid - 1;
    else
      return mid; // key found
  }
  return -(low + 1); // key not found.
}
```

# Safer

 * GCC never warns on integer sizing problems
 * Clang only warns at the -Weverything level
 * MSVC warns at the fairly normal /W3 level

This is a big annoyance for users porting to 64bit MSVC, but it's a real issue!

# Future Resistant

*Real World Example*

```cpp
// Not all compilers enforce all of the standard
void dosomething(std::string &t_str)
{ t_str = "code"; }

int main()
{
  dosomething(std::string("data"));
}
//
```

# Future Resistant

*Real World Example*

```cpp
// Not all compilers enforce all of the standard
void dosomething(std::string &t_str)
{ t_str = "code"; }

int main()
{
  dosomething(std::string("data")); // Compiles ONLY on MSVC
}
// Generates a warning only on /W4 or higher
```


# Future Resistant

 * Only MSVC allows non-const reference to temporary.
 * MSVC only warns all the up at /W4 level

Doing this is almost certainly a logic error, and is definitely a portability problem


# More Organized

 * OS specific code logically separated 
 * Leads to natural library / UI separation 

# More Tools Available

 * PVS Studio (Windows Only)
 * ReSharper C++ (Windows Only)
 * Valgrind (Linux / MacOS Only)
 * MSVC Static Analyzer (Windows Only)
 * Clang’s “sanitizers” (Linux is easiest)
 * Visual Studio's Debugger (Windows Only)

# Static Analysis

*From ChaiScript*

```cpp
template<typename T, typename U>
static Boxed_Value go(Operators::Opers t_oper, const T &t, const U &u, const Boxed_Value &) {
  switch (t_oper) {
    case Operators::equals:               return const_var(t == u);
    case Operators::less_than:            return const_var(t < u);
    case Operators::greater_than:         return const_var(t > u);
    case Operators::less_than_equal:      return const_var(t <= u);
    case Operators::greater_than_equal:   return const_var(t >= u);
    case Operators::not_equal:            return const_var(t != u);
    default:                              throw chaiscript::detail::exception::bad_any_cast();
  }
  throw chaiscript::detail::exception::bad_any_cast();
}
```



# Static Analysis

*From ChaiScript*

```cpp
template<typename T, typename U>
static Boxed_Value go(Operators::Opers t_oper, const T &t, const U &u, const Boxed_Value &) {
  switch (t_oper) {
    case Operators::equals:               return const_var(t == u);
    case Operators::less_than:            return const_var(t < u);
    case Operators::greater_than:         return const_var(t > u);
    case Operators::less_than_equal:      return const_var(t <= u);
    case Operators::greater_than_equal:   return const_var(t >= u);
    case Operators::not_equal:            return const_var(t != u);
    default:                              throw chaiscript::detail::exception::bad_any_cast();
  }
  throw chaiscript::detail::exception::bad_any_cast(); // caught by MSVC only
}
```

# Static Analysis 

```cpp
int main(int argc, char *[]) {
  if (argc > 3) {
    return 5;
  } else {
    return 5;
  }
}


```

# Static Analysis 

```cpp
int main(int argc, char *[]) {
  if (argc > 3) {
    return 5;
  } else {
    return 5;
  }
}
// testcppcheck.cpp:5] -> [testcppcheck.cpp:3]: 
//        (style, inconclusive) Found duplicate branches for 'if' and 'else'.
```



# Static Analysis

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

# Static Analysis

```cpp
if (rt.bare_equal(boxed_type))
{
  // CID 89289 (#1 of 1): Identical code for different branches (IDENTICAL_BRANCHES)
  if (lt.bare_equal(boxed_pod_type))
  {
    return true;
  }
  // implicit_else: The code from the above if-then branch is identical to the code after the 
  // if statement.
  return true;
}
```

From scan.coverity.com

*identical_branches: The same code is executed when the condition lt->bare_equal(boxed_pod_type) is true or false, because the code in the if-then branch and after the if statement is identical. Should the if statement be removed?*


# Wider Customer Base

 * iOS
 * Android
 * Linux
 * MacOS
 * Windows
 * Humble Bundle
 * SteamOS

# Guidelines - Build Tool

## DRY – Don't Repeat Yourself

 * Maintaining multiple project files for multiple build configurations is hard. Let a build tool / makefile generator do the work for you


# Guidelines - Build Tool

 * CMake 
 * biicode
 * qmake
 * premake
 * meson
 * Others here at the conference?


# Guidelines - Choose Your Compilers

 * Choose which compilers and how old you’ll support
 * VS 2013 has limited C++11 support (constexpr, nothrow, defaulted functions, magic statics... VS2015 is much better)
 * GCC 4.6 warns on return type deduction of complex lambdas
 * And other issues


# Guidelines - Choose Your Compilers

```cpp
#ifndef CHAISCRIPT_MSVC_12
#define CHAISCRIPT_HAS_MAGIC_STATICS
#endif

#if (defined(__GNUC__) && __GNUC__ > 4) || (__GNUC__ == 4 && __GNUC_MINOR__ >= 7) || defined(CHAISCRIPT_MSVC) || defined(__llvm__)
#define CHAISCRIPT_OVERRIDE override
#else
#define CHAISCRIPT_OVERRIDE
#endif

#ifdef CHAISCRIPT_MSVC
#define CHAISCRIPT_NOEXCEPT throw()
#define CHAISCRIPT_CONSTEXPR 
#else
#define CHAISCRIPT_NOEXCEPT noexcept
#define CHAISCRIPT_CONSTEXPR constexpr
#endif
```


# Guidelines - GUI Toolkit

 * Native?
 * wxWidgets
 * gtkmm
 * FLTK
 * Qt


# Guidelines - Filesystem Access

 * Qt
 * wxWidgets
 * Boost (>255 length issue?)
 * Wait for C++1z?
 * Roll your own (keep it as high level as possible)


# Guidelines - Automated Builds

You will never maintain cross platform capability without automated builds

 * TravisCI (http://travis-ci.org, Linux)
 * AppVeyor (http://appveyor.com, Windows)
 * Hudson / Jenkins etc
 * DecentCI (http://github.com/lefticus/decent_ci)


# Guidelines - Automated Analysis

 * Coverity Scan (https://scan.coverity.com, Linux/Windows)
 * Coveralls (https://coveralls.io)
 * Codecov (https://codecov.io)


# Guildelines - Dashboard

 * http://chaiscript.com/ChaiScript-BuildResults/full_dashboard.html
 * github pull requests


# How To Convince Your Team

 * Show them the examples from these slides
   * http://github.com/lefticus/presentations/ThinkingPortable.md
 * Try to extract some core functionality
 * Set up a CMake stub to compile the core
 * Demonstrate a tangible benefit from what the new compiler finds


# What If The Team Isn’t Convinced?

 * Make full use of the tools you do have available
 * Turn up warnings on current compiler (/W4, -Wall, -Wextra, -Weverything)
 * Enable static analysis with MSVC / Clang
 * Install Cppcheck
 * Enable automated builds
 * http://cppbestpractices.com


# What are the downsides?

 * You must pick a subset of the language that you’ll use
 * You must pick a subset of OS/GUI Functionality
 * Unexpected differences


# Unexpected Differences: Linking


<table>
<tr align='center'><td style="border:1px solid; border-color:#000000; background-color:#FFFF11;">Logger</td><td>&nbsp;&nbsp;&nbsp;</td><td style="border:1px solid; border-color:#000000; background-color:#FFFF11;">Logger</td><td>&nbsp;&nbsp;&nbsp;</td><td style="border:1px solid; border-color:#000000; background-color:#FFFF11;">Logger</td></tr>
<tr align='center'><td><small>static</small></td><td>&nbsp;&nbsp;&nbsp;</td><td><small>static</small></td><td>&nbsp;&nbsp;&nbsp;</td><td><small>static</small><td></tr>
<tr align='center'><td style="border:1px solid; border-color:#000000; background-color:#FFFF11;">Lib-1</td><td>&nbsp;&nbsp;&nbsp;</td><td style="border:1px solid; border-color:#000000; background-color:#FFFF11;">Lib-2</td><td>&nbsp;&nbsp;&nbsp;</td><td style="border:1px solid; border-color:#000000; background-color:#FFFF11;">Lib-3</td></tr>
<tr align='center'><td><small>dynamic</small></td><td>&nbsp;&nbsp;&nbsp;</td><td><small>dynamic</small></td><td>&nbsp;&nbsp;&nbsp;</td><td><small>dynamic</small><td></tr>
<tr align='center'><td style="border:1px solid; border-color:#000000; background-color:#FFFF11;">Ruby Lib-1</td><td>&nbsp;&nbsp;&nbsp;</td><td style="border:1px solid; border-color:#000000; background-color:#FFFF11;">Ruby Lib-2</td><td>&nbsp;&nbsp;&nbsp;</td><td style="border:1px solid; border-color:#000000; background-color:#FFFF11;">Ruby Lib-3</td></tr>
<tr align='center'><td colspan=5><small>Dynamic</small></td></tr>
<tr align='center'><td colspan=5 style="border:1px solid; border-color:#000000; background-color:#FFFF11;">Ruby</td></tr>
</table>


# Unexpected Difference: Linking

Global static logger object linked to 3 different dynamic libraries

> * Linux: 1 Logger
> * Windows: 3 Loggers
> * MacOS: 1 Logger – Freed 3 times (crash on shutdown)


# Unexpected Difference: Linking

 * Link a static library into your project at most 1 time.
 * Prefer either 100% dynamic or 100% static linking
 * Dynamically loading with a scripting engine might force you into 100% dynamic
 * Avoid singletons as much as possible.


# Sometimes you end up with this

```cpp
#ifdef CHAISCRIPT_MSVC_12
#pragma warning(push)
#pragma warning(disable : 6011)
#endif
          // this analysis warning is invalid in
          //MSVC12 and doesn't exist in MSVC14
          std::vector<Type_Info> retval{types[0]};
#ifdef CHAISCRIPT_MSVC_12
#pragma warning(pop)
#endif
```

Not just picking on MSVC here, every compiler has its quirks


# Jason Turner

 * http://chaiscript.com
 * http://cppbestpractices.com
 * http://github.com/lefticus
 * http://cppcast.com
 * @lefticus
 * Independent Contractor

Always interested in meeting new clients - contact me if you are interested in working with me.


