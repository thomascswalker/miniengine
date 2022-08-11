#include "color.h"

MColor::MColor(int r, int g, int b)
{
    m_rgb = ((r & 0xff) << 16) + ((g & 0xff) << 8) + (b & 0xff);
}
