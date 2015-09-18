#include "ChaiScript/include/chaiscript/chaiscript.hpp"
#include "chaiscript_stdlib.hpp"
#include <future>

int main()
{
  chaiscript::ChaiScript chai(create_chaiscript_stdlib());

  chai.add(chaiscript::var(std::async([](){ return chaiscript::var(6+5); })), "a_future");

  chai.eval(R"(
      print(a_future.get());
    )");

  std::cout << chai.eval<int>("async(fun(){ 6+6; }).get()") << '\n';

}

