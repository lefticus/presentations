#include <sol.hpp>
#include <cassert>
#include <iostream>

std::string hello( const std::string & input ) { 
  return "hello " + input;
}

int main() {
  sol::state lua;
  lua.open_libraries(sol::lib::base);

  // setting a function is simple
  lua.set_function("hello", hello);
  lua.script(R"(
    for i = 0,1000000,1  do print(hello("world")) end
)");

}
