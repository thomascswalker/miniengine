#ifndef COLOR_H
#define COLOR_H

#include <random>
#include "api.h"

MINI_NAMESPACE_OPEN
MINI_USING_DIRECTIVE

class Color
{
public:
	Color() {};
	Color(int rgb) : m_rgb(rgb) {};
	Color(int r, int g, int b);

	// Returns the hexadecimal format of the color
	unsigned int hex() { return m_rgb; }

	static Color random();
	static Color red() { return Color(255, 0, 0); }
	static Color green() { return Color(0, 255, 0); }
	static Color blue() { return Color(0, 0, 255); }

	static Color cyan() { return Color(0, 255, 255); }
	static Color magenta() { return Color(255, 0, 255); }
	static Color yellow() { return Color(255, 255, 0); }

	static Color purple() { return Color(128, 0, 255); }
	static Color orange() { return Color(255, 128, 0); }

	static Color white() { return Color(255, 255, 255); }
	static Color gray() { return Color(128, 128, 128); }
	static Color black() { return Color(0, 0, 0); }

private:
	unsigned int m_rgb = 0x000000;
};

MINI_NAMESPACE_CLOSE

#endif