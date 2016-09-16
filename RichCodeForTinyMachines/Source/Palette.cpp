#include <cstdint>
#include <array>
#include <algorithm>

namespace {
  struct Color
  {
    uint8_t num;
    uint8_t r;
    uint8_t g;
    uint8_t b;
  };


  volatile uint8_t& memory(const uint16_t loc)
  {
    return *reinterpret_cast<uint8_t*>(loc);
  }
  
  template<typename T>
    auto square(T t) {
      return t * t;
  }

  struct VIC_II
  {
    static constexpr uint16_t VIDEO_MEMORY = 1024;
    static constexpr uint16_t BORDER_COLOR     = 53280;
    static constexpr uint16_t BACKGROUND_COLOR = 53281;

    volatile uint8_t& border() {
      return memory(BORDER_COLOR);
    }
    
    volatile uint8_t& background() {
      return memory(BACKGROUND_COLOR);
    }
    
    volatile uint8_t& display(uint8_t x, uint8_t y)
    {
      return memory(VIDEO_MEMORY + y * 40 + x);
    }
    
    template<uint8_t r, uint8_t g, uint8_t b>
    static auto color_comparison(const Color &lhs, const Color &rhs)
    {
      // distance between colors:
      // sqrt( (r1 - r2)^2 + (g1 - g2)^2 + (b1 - b2)^2 )
      return (square(lhs.r - r) + square(lhs.g - g) + square(lhs.b - b))
            <(square(rhs.r - r) + square(rhs.g - g) + square(rhs.b - b));
    }

    template<uint8_t r, uint8_t g, uint8_t b, typename Colors>
    auto nearest_color(const Colors &colors)
    {

    }
  };  
}

int main()
{
  static std::array<Color, 16> colors {{
    Color{0,  0x00, 0x00, 0x00},
    Color{1,  0xFF, 0xFF, 0xFF},
    Color{2,  0x88, 0x39, 0x32},
    Color{3,  0x67, 0xB6, 0xBD},
    Color{4,  0x8B, 0x3F, 0x96},
    Color{5,  0x55, 0xA0, 0x49},
    Color{6,  0x40, 0x31, 0x8D},
    Color{7,  0xBF, 0xCE, 0x72},
    Color{8,  0x8B, 0x54, 0x29},
    Color{9,  0x57, 0x42, 0x00},
    Color{10, 0xB8, 0x69, 0x62},
    Color{11, 0x50, 0x50, 0x50},
    Color{12, 0x78, 0x78, 0x78},
    Color{13, 0x94, 0xE0, 0x89},
    Color{14, 0x78, 0x69, 0xC4},
    Color{15, 0x9F, 0x9F, 0x9F}
  }};
  
  VIC_II vic;
  vic.background() = 1;
  vic.border() = 12;
}

