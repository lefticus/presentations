/// Working with fixed Pallete 

///
/// Start:
///

#include <cstdint>
#include <array>

constexpr uint16_t SCREEN_BORDER_COLOR     = 53280;
constexpr uint16_t SCREEN_BACKGROUND_COLOR = 53281;

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
}

int main()
{
  std::array<Color, 16> colors {{
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
  
  memory(SCREEN_BACKGROUND_COLOR) = 0;
  memory(SCREEN_BORDER_COLOR) = 12;
}


/// 
/// Middle: 
///

#include <cstdint>
#include <algorithm>
#include <array>

constexpr uint16_t SCREEN_BORDER     = 53280;
constexpr uint16_t SCREEN_BACKGROUND = 53281;

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
  
  template<uint8_t r, uint8_t g, uint8_t b>
  auto color_distance(const Color &lhs, const Color &rhs)
  {
    return (square(lhs.r - r) + square(lhs.g - g) + square(lhs.b - b))
      < (square(rhs.r - r) + square(rhs.g - g) + square(rhs.b - b));
  }

  template<uint8_t r, uint8_t g, uint8_t b, typename Colors>
  auto nearest_color(const Colors &colors)
  {
    return *std::min_element(std::begin(colors), std::end(colors), color_distance<r,g,b>);
  }

}

int main()
{
  std::array<Color, 16> colors {{
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
  
  memory(SCREEN_BORDER) = nearest_color<128,128,128>(colors).num;
}


///
/// Final:
///

// HEY, WHO USES CONST WHENEVER POSSIBLE?!

#include <cstdint>
#include <algorithm>
#include <array>

constexpr uint16_t SCREEN_BORDER     = 53280;
constexpr uint16_t SCREEN_BACKGROUND = 53281;

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
  
  template<uint8_t r, uint8_t g, uint8_t b>
  auto color_distance(const Color &lhs, const Color &rhs)
  {
    return (square(lhs.r - r) + square(lhs.g - g) + square(lhs.b - b))
      < (square(rhs.r - r) + square(rhs.g - g) + square(rhs.b - b));
  }


  template<uint8_t r, uint8_t g, uint8_t b, typename Colors>
  auto nearest_color(const Colors &colors)
  {
    return *std::min_element(std::begin(colors), std::end(colors), color_distance<r,g,b>);
  }

}

int main()
{
  const std::array<Color, 16> colors {{
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
  
  memory(SCREEN_BORDER)     = nearest_color<128,128,128>(colors).num;
  memory(SCREEN_BACKGROUND) = nearest_color<0,0,0>(colors).num;
}

