# The Current State of (Free) Static Analysis

*Why and How to make your C++ Cross Platform*

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

 * We will be focusing on
   * source code analysis
   * tools that are freely available
 * Technically this includes compiler warnings
   * compiler warnings will only be brought up if they are unique to a particular compiler

# Tools

```cpp
int main()
{
  std::cout << "bob\n";
}






asdf

sadf


sadf


sadf

asdf

```
