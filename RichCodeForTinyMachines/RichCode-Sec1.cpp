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
  memory(53280) = 1;
  memory(53280) = 2;
}


