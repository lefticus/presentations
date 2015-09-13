#include "ChaiScript/include/chaiscript/chaiscript.hpp"
#include "chaiscript_stdlib.hpp"


int main()
{
  chaiscript::ChaiScript chai(create_chaiscript_stdlib());

  // Pass a C++ lambda into ChaiScript
  chai.add(chaiscript::fun([](const std::string &t_name) { std::cout << "Hello " << t_name << '\n'; }), "hello_from_cpp");

  // Call It.
  chai.eval(R"(
    hello_from_cpp("C++ World");
  )");


  // Pass a ChaiScript Lambda back into C++
  const auto hello_from_chai = chai.eval<std::function<void (const std::string &)>>( R"(
    fun(s){ print("Hello ${s}"); }
  )");

  // Call It.
  hello_from_chai("ChaiScript World");

}

