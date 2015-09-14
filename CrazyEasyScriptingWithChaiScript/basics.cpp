#include "ChaiScript/include/chaiscript/chaiscript.hpp"
#include "chaiscript_stdlib.hpp"



int main()
{
  chaiscript::ChaiScript chai(create_chaiscript_stdlib());

  chai.eval(R"(
    print("hello world");
  )");
}

