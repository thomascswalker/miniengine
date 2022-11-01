#include "pixel.h"

MINI_NAMESPACE_OPEN
MINI_USING_DIRECTIVE

int Pixel::hex()
{
    auto h = ((_r & 0xff) << 16 + (_g & 0xff) << 8 + (_b & 0xff));
    return h;
}

MINI_NAMESPACE_CLOSE