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
}

int main() {
  memory(1024 + 10 + (11 * 40)) = ‘a’;
  memory(1024 + 20 + (11 * 40)) = ‘b’;
}


///
/// End With:
///

#include <cstdint>

constexpr uint16_t VIDEO_MEMORY = 1024;

namespace {
  volatile uint8_t &memory(const uint16_t loc)
  {
    return *reinterpret_cast<uint8_t *>(loc);
  }

  void display(uint8_t x, uint8_t y, uint8_t val)
  {
    memory(VIDEO_MEMORY + y * 40 + x) = val;
  }
}

int main() {
  display(10, 11, ‘a’);
  display(20, 22, ‘b’);
}
