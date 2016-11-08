
#include <cstdint>

int main()
{
  *reinterpret_cast<uint8_t*>(53280) = 1;
}

