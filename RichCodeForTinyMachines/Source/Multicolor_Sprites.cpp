#include <cstdint>
#include <array>
#include <utility>
#include <algorithm>

constexpr uint8_t  SPRITE_STARTING_BANK = 192;


namespace {

  volatile uint8_t &memory(const uint16_t loc)
  {
    return *reinterpret_cast<uint8_t *>(loc);
  }

  void write_multi_color_pixel(uint16_t)
  {
    // 0th case
  }

  void write_pixel(uint16_t)
  {
    // 0th case
  }

  template<typename ...  D >
    void write_multi_color_pixel(uint16_t loc, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, D ... d)
  {
    memory(loc) = (d1 << 6) | (d2 << 4) | (d3 << 2) | d4;
    write_multi_color_pixel(loc + 1, d...);
  }

  template<typename ...  D >
    void write_pixel(uint16_t loc, bool d1, bool d2, bool d3, bool d4, bool d5, bool d6, bool d7, bool d8, D ... d)
  {
    memory(loc) = (d1 << 7) | (d2 << 6) | (d3 << 5) | (d4 << 4) | (d5 << 3) | (d6 << 2) | (d7 << 1) | d8;
    write_pixel(loc + 1, d...);
  }

  template<typename ... D>
    void make_sprite(uint8_t memory_loc, D ... d)
  {
    if (sizeof...(d) == 12 * 21) {
      write_multi_color_pixel((SPRITE_STARTING_BANK + memory_loc) * 64, d...);
    } else {
      write_pixel((SPRITE_STARTING_BANK + memory_loc) * 64, d...);
    }
  }
  
}

int main()
{  
  make_sprite(1,
              0,0,0,0,0,2,2,0,0,0,0,0,
              0,0,0,0,0,2,2,0,0,0,0,0,
              0,0,0,0,0,2,2,0,0,0,0,0,
              0,0,0,0,0,2,2,0,0,0,0,0,
              0,0,0,0,0,2,2,0,0,0,0,0,
              0,0,0,0,0,2,2,0,0,0,0,0,
              0,0,0,0,0,2,2,0,0,0,0,0,
              0,0,0,0,0,2,2,0,0,0,0,0,
              0,0,0,0,0,2,2,0,0,0,0,0,
              0,0,0,0,0,2,2,0,0,0,0,0,
              0,0,0,0,0,2,2,0,0,0,0,0,
              0,0,0,0,0,2,2,0,0,0,0,0,
              0,0,0,0,0,2,2,0,0,0,0,0,
              0,0,0,0,0,2,2,0,0,0,0,0,
              0,0,0,0,0,2,2,0,0,0,0,0,
              0,0,0,0,0,2,2,0,0,0,0,0,
              0,0,0,0,0,3,3,0,0,0,0,0,
              0,0,0,0,0,1,1,0,0,0,0,0,
              0,0,0,0,0,3,3,0,0,0,0,0,
              0,0,0,0,0,1,1,0,0,0,0,0,
              0,0,0,0,0,3,3,0,0,0,0,0
             );
}

