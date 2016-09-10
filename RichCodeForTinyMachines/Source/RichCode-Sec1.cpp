/// Section 1 
/// Memory

///
/// Start with: 
///


#include <cstdint>

int main()
{
  *reinterpret_cast<uint8_t*>(53280) = 1;
}

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
/// Mid 3
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

///
/// Final
///

#include <cstdint>

namespace {
  volatile uint8_t &memory(const uint16_t loc)
  {
    return *reinterpret_cast<uint8_t *>(loc);
  }
  
  struct VIC_II
  {
    static constexpr uint16_t BORDER_COLOR = 0xd020;
    volatile uint8_t &border() {
      return memory(BORDER_COLOR);
    }
  };
}

int main()
{
  VIC_II vic;
  vic.border() = 1;
  vic.border() = 2;
}
