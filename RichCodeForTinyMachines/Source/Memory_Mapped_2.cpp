
#include <cstdint>

namespace {
  volatile uint8_t &memory(const uint16_t loc)
  {
    return *reinterpret_cast<uint8_t*>(loc);
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

