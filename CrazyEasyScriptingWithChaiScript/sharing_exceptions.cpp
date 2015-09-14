#include "ChaiScript/include/chaiscript/chaiscript.hpp"
#include "chaiscript_stdlib.hpp"


void throw_something()
{
  throw std::runtime_error("a C++ exception");
}

int main()
{
  chaiscript::ChaiScript chai(create_chaiscript_stdlib());

  // register a C++ function that throws an exception
  chai.add(chaiscript::fun(&throw_something), "throw_something");

  chai.eval(R"(

    try {
      throw_something()
    } catch (e) {
      print("Caught: ${e.what()}");
    }
  )");


  // Throw a C++ function from chaiscript and catch it in C++
  try {
    chai.eval(R"(
      throw(runtime_error("a ChaiScript exception"));
    )", chaiscript::exception_specification<std::runtime_error>());
  } catch (const std::runtime_error &e) {
    std::cout << "Caught: " << e.what() << '\n';
  }

}

