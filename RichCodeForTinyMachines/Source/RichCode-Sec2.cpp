/// Video Memory

///
/// Start With:
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
    
    volatile uint8_t& border() {
      return memory(BORDER_COLOR);
    }
  };
}

int main() {
  memory(1024 + 10 + (11 * 40)) = 'a';
  memory(1024 + 20 + (11 * 40)) = 'b';
}


///
/// End With:
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
    static constexpr uint16_t VIDEO_MEMORY = 1024;

    volatile uint8_t& border() {
      return memory(BORDER_COLOR);
    }
    
    volatile uint8_t& display(uint8_t x, uint8_t y)
    {
      return memory(VIDEO_MEMORY + y * 40 + x);
    }  
  };
  

}

int main() {
  VIC_II vic;
  vic.display(10, 11) = 'a';
  vic.display(20, 11) = 'b';
}
