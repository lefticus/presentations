#include "ChaiScript/include/chaiscript/chaiscript.hpp"
#include "chaiscript_stdlib.hpp"


int main()
{
  chaiscript::ChaiScript chai(create_chaiscript_stdlib());

  // Pass a c++ variable to chaiscript
  std::string s;
  chai.add(chaiscript::var(&s), "s");

  chai.eval(R"(
    s = "hello world from chaiscript";
  )");

  std::cout << s << '\n';


  // Pass a chaiscript variable to c++
  chai.eval(R"(
    var s2 = "";
  )");

  std::string &s2 = chai.eval<std::string &>("s2");
  s2 = "Hello world from C++";

  chai.eval(R"(
    print(s2);
  )");

}

