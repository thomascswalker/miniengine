#ifndef COLOR_H
#define COLOR_H
#endif

#include <random>

class Color
{
public:
	Color() {};
	Color(int rgb) : m_rgb(rgb) {};
	Color(int r, int g, int b);

	// Returns the hexadecimal format of the color
	unsigned int hex() { return m_rgb; }

	static Color random();

private:
	unsigned int m_rgb = 0x000000;
};
