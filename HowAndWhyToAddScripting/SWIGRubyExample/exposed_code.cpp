#include "exposed_code.hpp"

std::string hello( const int ) {
  return "hello world";
}

std::string hello( const std::string & input ) { 
  return "hello " + input;
}


