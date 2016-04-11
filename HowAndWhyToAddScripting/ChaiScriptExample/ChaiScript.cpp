#include <chaiscript/chaiscript.hpp>
#include <chaiscript/chaiscript_stdlib.hpp>

std::string hello( const std::string & input ) { 
  return "hello " + input;
}

int main()
{
  chaiscript::ChaiScript chai(chaiscript::Std_Lib::library());

  chai.add(chaiscript::fun(&hello), "hello");
  chai.eval(R"(print(hello("world"));)");

}

