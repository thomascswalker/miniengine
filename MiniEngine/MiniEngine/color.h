#ifndef COLOR_H
#define COLOR_H
#endif

#include <random>

class MColor
{
public:
	MColor() {};
	MColor(int rgb) : m_rgb(rgb) {};
	MColor(int r, int g, int b);

	// Returns the hexadecimal format of the color
	unsigned int hex() { return m_rgb; }

	static MColor random();

private:
	unsigned int m_rgb = 0x000000;
};
