#include "ChaiScript/include/chaiscript/chaiscript.hpp"
#include "chaiscript_stdlib.hpp"


struct Config
{
  int num_steps;
  int repeat_times;
  std::string description;
};


int main()
{
  chaiscript::ChaiScript chai(create_chaiscript_stdlib());

  chai.add(chaiscript::fun(&Config::num_steps), "num_steps");
  chai.add(chaiscript::fun(&Config::repeat_times), "repeat_times");
  chai.add(chaiscript::fun(&Config::description), "description");
  chai.add(chaiscript::constructor<Config()>(), "Config");

  Config c = chai.eval_file<Config>("config.chai");

  std::cout << c.num_steps << '\n'
            << c.repeat_times << '\n'
            << c.description << '\n';


}

