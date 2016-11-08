
///
/// Initial Code:
///

#include <cstdint>
#include <utility>


namespace {
  volatile uint8_t &memory(const uint16_t loc)
  {
    return *reinterpret_cast<uint8_t *>(loc);
  }

  constexpr bool test_bit(const uint8_t data, const uint8_t bit)
  {
    return (data & (1 << bit)) != 0;
  };


  struct JoyStick
  {
    static constexpr uint16_t JOYSTICK_PORT_A = 56320; // joystick #2
    static constexpr uint16_t JOYSTICK_PORT_B = 56321; // joystick #1
    
    struct PortData
    {
      uint8_t data;
    };
    
    JoyStick(const PortData d)
      : up(!test_bit(d.data, 0)),
        down(!test_bit(d.data, 1)),
        left(!test_bit(d.data, 2)),
        right(!test_bit(d.data, 3)),
        fire(!test_bit(d.data, 4))
    {
    }
    
    JoyStick(const uint8_t port_num)
      : JoyStick(PortData{port_num==2?memory(JOYSTICK_PORT_A):memory(JOYSTICK_PORT_B)})
    {
    }
    
    auto direction_vector() const
    {
      return std::make_pair(left?-1:(right?1:0), up?-1:(down?1:0));
    }
    
    bool up,down,left,right,fire;
  };

  struct VIC_II
  {
    static constexpr uint16_t BORDER_COLOR     = 53280;
    static constexpr uint16_t BACKGROUND_COLOR = 53281;
    static constexpr uint16_t SCREEN_RASTER_LINE = 53266;

    volatile uint8_t& border() {
      return memory(BORDER_COLOR);
    }
    
    volatile uint8_t& background() {
      return memory(BACKGROUND_COLOR);
    }
  };

}

int main()
{
  VIC_II vic;

  while (true) {
    if (const auto joy = JoyStick(1); joy.fire) 
    {
      ++vic.background();
    }
  }
}



///
/// Middle Code:
///

#include <cstdint>
#include <utility>


namespace {
  volatile uint8_t &memory(const uint16_t loc)
  {
    return *reinterpret_cast<uint8_t *>(loc);
  }

  constexpr bool test_bit(const uint8_t data, const uint8_t bit)
  {
    return (data & (1 << bit)) != 0;
  };


  struct JoyStick
  {
    static constexpr uint16_t JOYSTICK_PORT_A = 56320; // joystick #2
    static constexpr uint16_t JOYSTICK_PORT_B = 56321; // joystick #1
    
    struct PortData
    {
      uint8_t data;
    };
    
    JoyStick(const PortData d)
      : up(!test_bit(d.data, 0)),
        down(!test_bit(d.data, 1)),
        left(!test_bit(d.data, 2)),
        right(!test_bit(d.data, 3)),
        fire(!test_bit(d.data, 4))
    {
    }
    
    JoyStick(const uint8_t port_num)
      : JoyStick(PortData{port_num==2?memory(JOYSTICK_PORT_A):memory(JOYSTICK_PORT_B)})
    {
    }
    
    auto direction_vector() const
    {
      return std::make_pair(left?-1:(right?1:0), up?-1:(down?1:0));
    }
    
    bool up,down,left,right,fire;
  };

  struct VIC_II
  {
    static constexpr uint16_t BORDER_COLOR     = 53280;
    static constexpr uint16_t BACKGROUND_COLOR = 53281;
    static constexpr uint16_t SCREEN_RASTER_LINE = 53266;

    volatile uint8_t& border() {
      return memory(BORDER_COLOR);
    }
    
    volatile uint8_t& background() {
      return memory(BACKGROUND_COLOR);
    }
  };

}

int main()
{
  VIC_II vic;

  while (true) {
    while (memory(VIC_II::SCREEN_RASTER_LINE) != 250) {}
    
    if (const auto joy = JoyStick(1); joy.fire) 
    {
      ++vic.background();
    }
    
    ++vic.border();
  }
}



///
/// Final Code:
///

#include <cstdint>
#include <utility>


namespace {
  volatile uint8_t &memory(const uint16_t loc)
  {
    return *reinterpret_cast<uint8_t *>(loc);
  }

  constexpr bool test_bit(const uint8_t data, const uint8_t bit)
  {
    return (data & (1 << bit)) != 0;
  };


  struct JoyStick
  {
    static constexpr uint16_t JOYSTICK_PORT_A = 56320; // joystick #2
    static constexpr uint16_t JOYSTICK_PORT_B = 56321; // joystick #1
    
    struct PortData
    {
      uint8_t data;
    };
    
    JoyStick(const PortData d)
      : up(!test_bit(d.data, 0)),
        down(!test_bit(d.data, 1)),
        left(!test_bit(d.data, 2)),
        right(!test_bit(d.data, 3)),
        fire(!test_bit(d.data, 4))
    {
    }
    
    JoyStick(const uint8_t port_num)
      : JoyStick(PortData{port_num==2?memory(JOYSTICK_PORT_A):memory(JOYSTICK_PORT_B)})
    {
    }
    
    auto direction_vector() const
    {
      return std::make_pair(left?-1:(right?1:0), up?-1:(down?1:0));
    }
    
    bool up,down,left,right,fire;
  };

  struct VIC_II
  {
    static constexpr uint16_t BORDER_COLOR     = 53280;
    static constexpr uint16_t BACKGROUND_COLOR = 53281;
    static constexpr uint16_t SCREEN_RASTER_LINE = 53266;

    volatile uint8_t& border() {
      return memory(BORDER_COLOR);
    }
    
    volatile uint8_t& background() {
      return memory(BACKGROUND_COLOR);
    }
    
    auto frame()
    {
      struct Frame
      {
        Frame() {
          while (memory(SCREEN_RASTER_LINE) != 250) {}
        }
        
        ~Frame() {
          VIC_II vic;
          ++vic.border();
        }
        
        Frame(const Frame &) = delete;
        Frame &operator=(const Frame &) = delete;
        Frame(Frame &&) = default;
        Frame &operator=(Frame &&) = default;
      };
      
      return Frame();
    }
  };

}

int main()
{
  VIC_II vic;

  while (true) {
    auto frame = vic.frame();
    
    if (const auto joy = JoyStick(1); joy.fire) 
    {
      ++vic.background();
    }
  }
}
