-----------------------------------

# How And Why To Add Scripting


-----------------------------------

# Jason Turner

 * http://github.com/lefticus/presentations
 * http://cppcast.com
 * http://chaiscript.com
 * http://cppbestpractices.com
 * C++ Weekly
 * @lefticus
 * Independent Contractor

-----------------------------------

# My Scripting Background

 * First embedded script engine in C++ for a distributed Command and Control network, using Lua via SWIG
 * Created Swig Starter Kit November 2008
 * Started work on ChaiScript May 2009
 * Consulted on C++/Scripting projects since 2010
 * Contributed to SWIG / Node/V8 binding generator


-----------------------------------


# Why Do You Want Scripting?

I meet two kinds of C++ developers:

> - Those who are already using scripting
> - Those who have no idea why one would want scripting

. . .

Today we are going to focus on calling script from your C++, not calling C++ from your script

-----------------------------------

# Why Do You Want Scripting?

## Config Files

 - Any application of any real complexity is going to need runtime configuration
 - Often this ends up being a very simple, easy to parse file

-----------------------------------

# Why Do You Want Scripting?

## Config Files

Homebrew INI file

```Ini
widget_1_x = 5
widget_1_y = 5
widget_1_width = 10
widget_1_height = 10
widget_1_name = "widget1"

widget_2_x = 15
widget_2_y = 5
widget_2_width = 10
widget_2_height = 10
widget_2_name = "widget2"
```

-----------------------------------

# Why Do You Want Scripting?

## Config Files

When what you really mean is

```Ini
widget_count = 2

widget_1_x = 5
widget_1_y = 5
widget_1_width = 10
widget_1_height = 10
widget_1_name = "widget1"

widget_2_x = widget_1_x + widget_1_width
widget_2_y = widget_1_y
widget_2_width = 10
widget_2_height = 10
widget_2_name = "widget2"
```


-----------------------------------

# Why Do You Want Scripting?

## Config Files

Scripted Config File

```
widget1 = Widget(5, 5, 
                 10, 10, "widget1");

widget2 = Widget(widget1.x + widget1.width, widget1.y, 
                 10, 10, "widget2");

add_widget(widget1);
add_widget(widget2);
```

-----------------------------------

# Why Do You Want Scripting?

## Config Files

By using a scripting engine we:

> - gain flexibility
> - save the effort of writing a parser
> - can express our C++ types in our config files


-----------------------------------


# Why Do You Want Scripting?

## Application Logic

 * By scripting application logic you can get much faster cycles for tweaking logic without recompiling
 * You can use scripted application logic as a prototype, then convert to C++ when performance becomes an issue

-----------------------------------


# Why Do You Want Scripting?

## User Extensibility

 * Common in javascript based applications
 * github's atom editor
 * etc

Our C++ applications can have the same level of flexibility and extensibility



-----------------------------------


# Languages Designed For Embedding 

 - Lua
 - ChaiScript
 - V8
 - Qt Script
 - Angelscript
 - etc

-----------------------------------

# Scripting Languages That Can Be Embedded

 - Ruby
 - Python
 - etc

-----------------------------------


# Tools We'll Cover

> - SWIG: Simplified Wrapper and Interferface Generator
> - Boost.Python: Python bindings interface layer provided by Boost
> - sol2: Modern C++ bindings for Lua
> - ChaiScript: Embedded scripting engine designed for C++

-----------------------------------


# SWIG

 - Parses C++ and generates bindings for various languages
 - Last updated: 2015-12-31 - in active development

# SWIG

Extensive Language Support

```text
 Allegro CL             C#
 CFFI                   CLISP
 Chicken                D
 Go                     Guile
 Java                   Javascript
 Lua                    Modula-3
 Mzscheme               OCAML
 Octave                 Perl
 PHP                    Python
 R                      Ruby
 Scilab                 Tcl
 UFFI 
```


# SWIG

## Advantages

> * Mostly automated, you don't have to specify your own interface (but can choose to)
> * The generator can automatically create 'directors' to allow you to inherit from C++ classes in your script
> * Can be configured to marshall exceptions between target language and C++

## Disadvantages

> * Multiple build steps with a code generator
> * SWIG adds its own layer of indirection to handle overloads, which adds overhead
> * Marshalling of exceptions can add a lot of generated code
> * Sometimes SWIG can be very sensitive to type definition ordering

# SWIG - Usage

 1. Specify C++ interface you want exposed to your scripting language
 2. Execute SWIG which generates a wrapper file
 3. Compile generated SWIG output file
 4. Initialize embedded scripting engine and load SWIG generated module
 5. Execute script


# SWIG/Ruby - C++ Interface

```cpp
#ifndef EXPOSED_CODE_HPP
#define EXPOSED_CODE_HPP

#include <string>

std::string hello( const std::string & input );

#endif
```


# SWIG/Ruby - SWIG .i Interface

```swig
%module EmbeddedScripting

%include <std_string.i>
%include "exposed_code.hpp"

%{
#include "exposed_code.hpp"
%}
```


# SWIG/Ruby - C++ Embedding

```cpp
extern "C" {
  // needed to provide the signature for initing our own module
  // this needs to match the signature of the module generated by SWIG
  void Init_EmbeddedScripting(void);
}

int main(int argc, char *argv[]) {

  // ruby initialization for embedding is completely undocument from what we could find
  // this code is based on reading the source for the official irb
  ruby_sysinit(&argc, &argv);
  {
    RUBY_INIT_STACK;
    ruby_init();
  }

  Init_EmbeddedScripting();

  // This function defined elsewhere, available on github
  evalString(R"ruby(1000000.times { puts(EmbeddedScripting::hello('world')) })ruby");
  // Let's make this form of a raw string literal standard for syntax highlighting!
}
```


# Boost.Python

 - Provides a wrapper layer for boost <-> python
 - Last updated: 2009-11-17 (AKA boost 1.41.0) according to boost release notes
 - Why didn't we use pybind11? Learned about it late into preparing this talk and couldn't find examples on how to embed (instead of create module).
 
# Boost.Python

## Advantages

> * Simple build process
> * Easy to use interface

## Disadvantages

> * Must specify each thing you want bound to Python, no generator
> * Not actively maintained

# Boost.Python - Usage

 1. Bind C++ functions to Python functions
 2. Initialize embedded scripting engine
 3. Load internally created module
 4. Execute script


# Boost.Python - Module Interface

```cpp
#include <boost/python.hpp>

std::string hello( const std::string & input ) { 
  return "hello " + input;
}

BOOST_PYTHON_MODULE(CppMod) {
  boost::python::def("hello", &hello);
}
```

# Boost.Python - C++ Embedding

```cpp
int main() {
  try {
    PyImport_AppendInittab( "CppMod", &initCppMod );
    Py_Initialize();

    boost::python::object main_module((
          boost::python::handle<>(boost::python::borrowed(PyImport_AddModule("__main__")))));

    boost::python::object main_namespace = main_module.attr("__dict__");
    boost::python::object cpp_module( (boost::python::handle<>(PyImport_ImportModule("CppMod"))) );

    main_namespace["CppMod"] = cpp_module;

    boost::python::handle<> ignored(( PyRun_String( 
R"python(
for number in range(1000000):    
    print(CppMod.hello(\"world\"))
)python",
            Py_file_input,
            main_namespace.ptr(),
            main_namespace.ptr() ) ));
  } catch ( const boost::python::error_already_set & ) {
    PyErr_Print();
  }
}
```


# sol2

 - Provides a wrapper layer between lua<->c++
 - Last release 2016-05-03 - actively developed

# sol2

## Advantages

> * Simple build process
> * Easy to use interface
> * Natural interaction with C++

## Disadvantages

> * Must specify each thing you want bound to Python, no generator
 
# sol2 - Usage

 1. Create lua state object
 2. Register C++ objects
 3. Execute script


# sol2 - C++ Embedding

```cpp
#include <sol.hpp>
#include <cassert>
#include <iostream>

std::string hello( const std::string & input ) { 
  return "hello " + input;
}

int main() {
  sol::state lua;
  lua.open_libraries(sol::lib::base);

  lua.set_function("hello", hello);
  lua.script(R"lua(
    for i = 0,1000000,1  do print(hello("world")) end
)lua");
}
```

# ChaiScript

 - Embedded scripting language co-designed by me specifically for C++
 - Last release 2016-04-31 - actively developed
 

# ChaiScript

## Advantages 

> - Header only - no external deps
> - Designed for integration with C++

## Disadvantages

> - Header only - compile times seem slow (but realisically probably not impact a real project much)

 
# ChaiScript - Usage

 1. Create ChaiScript engine object
 2. Register C++ objects
 3. Execute script





# ChaiScript - C++ Embedding

```cpp
#include <chaiscript/chaiscript.hpp>
#include <chaiscript/chaiscript_stdlib.hpp>

std::string hello( const std::string & input ) { 
  return "hello " + input;
}

int main()
{
  chaiscript::ChaiScript chai(chaiscript::Std_Lib::library());

  chai.add(chaiscript::fun(&hello), "hello");
  chai.eval(R"chaiscript(for(var i = 0; i < 1000000; ++i) { print(hello("world")); } )chaiscript");
}
```


# Conclusions

> - I don't recommend embedding either Ruby or Python
> - `PyErr_Print(); // global state`
> - Global state means multithreading is somewhere between very difficult and impossible
> - But there might be institutional reasons why either makes sense
>     - Existing code bases
>     - Existing knowledge bases
> - Just because Ruby isn't recommended doesn't mean SWIG is not
>     - SWIG / Lua, SWIG / V8 are good options


---------------------------------------------

# Questions?

## Jason Turner

 * http://github.com/lefticus/presentations
 * http://cppcast.com
 * http://chaiscript.com
 * http://cppbestpractices.com
 * C++ Weekly
 * @lefticus
 * Independent Contractor




