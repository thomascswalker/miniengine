#include "pixel.h"

union COLOR
{
    unsigned long RGB;
    std::byte bytes[3];
};

int Pixel::hex()
{
    auto h = ((_r & 0xff) << 16 + (_g & 0xff) << 8 + (_b & 0xff));
    return h;
}
