/// Section 1 
/// Memory

///
/// Start with: 
///

// clean slate


///
/// Mid point:
///

#include <cstdint>

uint8_t& memory(const uint16_t loc)
{
  return *reinterpret_cast<uint8_t*>(loc);
}

int main()
{
  memory(53280) = 1;
  memory(53280) = 2;
}

///
/// Mid 2
///

#include <cstdint>

volatile uint8_t& memory(const uint16_t loc)
{
  return *reinterpret_cast<uint8_t*>(loc);
}

int main()
{
  memory(53280) = 1;
  memory(53280) = 2;
}



///
/// End with:
///

#include <cstdint>

namespace {
  volatile uint8_t& memory(const uint16_t loc)
  {
    return *reinterpret_cast<uint8_t*>(loc);
  }
}

int main()
{
  const auto set_border = [](const auto color){
    memory(53280) = color;
  };
  
  set_border(1);
  set_border(2);
}
