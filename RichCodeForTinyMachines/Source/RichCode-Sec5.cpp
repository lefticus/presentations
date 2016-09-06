
///
/// Initial Code:
///

#include <cstdint>
#include <utility>

constexpr uint16_t JOYSTICK_PORT_A           = 56320; // joystick #2
constexpr uint16_t JOYSTICK_PORT_B           = 56321; // joystick #1
constexpr uint16_t SCREEN_BACKGROUND_COLOR   = 53280;
constexpr uint16_t SCREEN_RASTER_LINE        = 53266;
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

  constexpr bool test_bit(const uint8_t data, const uint8_t bit)
  {
    return (data & (1 << bit)) != 0;
  }


  auto joystick(const uint8_t port) {
    struct State{
      State(const uint8_t portdata) 
        : up(!test_bit(portdata,0)),
          down(!test_bit(portdata,1)),
          left(!test_bit(portdata,2)),
          right(!test_bit(portdata,3)),
          fire(!test_bit(portdata,4))
      {
      }

      bool up;
      bool down;
      bool left;
      bool right;
      bool fire;      
    };

    if (port == 2) {
      return State(memory(JOYSTICK_PORT_A));
    } else {
      return State(memory(JOYSTICK_PORT_B));
    }
  }
}

int main()
{
  while (true) {
    if (const auto joy = joystick(1); joy.fire && joy.up) 
    {
      ++memory(SCREEN_BACKGROUND_COLOR);
    }
  }
}



///
/// Middle Code:
///

#include <cstdint>
#include <utility>

constexpr uint16_t JOYSTICK_PORT_A           = 56320; // joystick #2
constexpr uint16_t JOYSTICK_PORT_B           = 56321; // joystick #1
constexpr uint16_t SCREEN_BACKGROUND_COLOR   = 53280;
constexpr uint16_t SCREEN_RASTER_LINE        = 53266;
constexpr uint16_t VIDEO_MEMORY = 1024;


namespace {
  volatile uint8_t &memory(const uint16_t loc)
  {
    return *reinterpret_cast<uint8_t *>(loc);
  }

  constexpr bool test_bit(const uint8_t data, const uint8_t bit)
  {
    return (data & (1 << bit)) != 0;
  }
  
  void display(uint8_t x, uint8_t y, uint8_t val)
  {
    memory(VIDEO_MEMORY + y * 40 + x) = val;
  }

  auto joystick(const uint8_t port) {
    struct State{
      State(const uint8_t portdata) 
        : up(!test_bit(portdata,0)),
          down(!test_bit(portdata,1)),
          left(!test_bit(portdata,2)),
          right(!test_bit(portdata,3)),
          fire(!test_bit(portdata,4))
      {
      }

      bool up;
      bool down;
      bool left;
      bool right;
      bool fire;      
    };

    if (port == 2) {
      return State(memory(JOYSTICK_PORT_A));
    } else {
      return State(memory(JOYSTICK_PORT_B));
    }
  }
}

int main()
{
  while (true) {
    while (memory(SCREEN_RASTER_LINE) != 250) {}
    
    if (const auto joy = joystick(1); joy.fire && joy.up) 
    {
      ++memory(SCREEN_BACKGROUND_COLOR);
    }
    
    display(20, 11, memory(SCREEN_BACKGROUND_COLOR));
  }
}



///
/// Final Code:
///

#include <cstdint>
#include <utility>

constexpr uint16_t JOYSTICK_PORT_A           = 56320; // joystick #2
constexpr uint16_t JOYSTICK_PORT_B           = 56321; // joystick #1
constexpr uint16_t SCREEN_BACKGROUND_COLOR   = 53280;
constexpr uint16_t SCREEN_RASTER_LINE        = 53266;
constexpr uint16_t VIDEO_MEMORY = 1024;


namespace {
  volatile uint8_t &memory(const uint16_t loc)
  {
    return *reinterpret_cast<uint8_t *>(loc);
  }

  constexpr bool test_bit(const uint8_t data, const uint8_t bit)
  {
    return (data & (1 << bit)) != 0;
  }
  
  void display(uint8_t x, uint8_t y, uint8_t val)
  {
    memory(VIDEO_MEMORY + y * 40 + x) = val;
  }


  auto joystick(const uint8_t port) {
    struct State{
      State(const uint8_t portdata) 
        : up(!test_bit(portdata,0)),
          down(!test_bit(portdata,1)),
          left(!test_bit(portdata,2)),
          right(!test_bit(portdata,3)),
          fire(!test_bit(portdata,4))
      {
      }

      bool up;
      bool down;
      bool left;
      bool right;
      bool fire;      
    };

    if (port == 2) {
      return State(memory(JOYSTICK_PORT_A));
    } else {
      return State(memory(JOYSTICK_PORT_B));
    }
  }
}

int main()
{
  struct Frame
  {
    Frame() 
    {
      while (memory(SCREEN_RASTER_LINE) != 250) {}
    } 
    
    ~Frame()
    {
      display(20, 11, memory(SCREEN_BACKGROUND_COLOR));
    }
  };
  
  while (true) {
    Frame f;
    if (const auto joy = joystick(1); joy.fire && joy.up) 
    {
      ++memory(SCREEN_BACKGROUND_COLOR);
    }
  }
}

