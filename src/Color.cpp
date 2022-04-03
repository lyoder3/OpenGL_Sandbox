#include "Color.h"
#include<iostream>

Color::Color(RGBColor col)
	: m_Red(col.red), m_Green(col.green), m_Blue(col.blue)
{
	m_HexCode = ConvertRGBToHex({m_Red, m_Green, m_Blue});
}

Color::Color(unsigned int hexCode)
{
	m_HexCode = hexCode;

	RGBColor col = ConvertHexToRGB(hexCode);
	m_Red = col.red;
	m_Green = col.green;
	m_Blue = col.blue;	
}

Color::~Color()
{
}

RGBColor Color::GetRGB() const
{
	return {m_Red, m_Green, m_Blue};
}

std::stringstream Color::GetHexString() const
{
	std::stringstream ss;

	ss << std::hex << m_HexCode;

	return ss;
}

unsigned int Color::ConvertRGBToHex(RGBColor col) const
{
	unsigned int red = (unsigned int) (255.0 * col.red);
	unsigned int green = (unsigned int) (255.0 * col.green);
	unsigned int blue = (unsigned int) (255.0 * col.blue);

	return ((red << 16) + (green << 8) + blue);
}

RGBColor Color::ConvertHexToRGB(unsigned int hexCode) const
{
	unsigned int redCode = (hexCode >> 16) & 0xFF;
	unsigned int greenCode = (hexCode >> 8) & 0xFF;
	unsigned int blueCode = hexCode & 0xFF;

	float red = (float) redCode / 255.0f;
	float green = (float) greenCode / 255.0f;
	float blue = (float) blueCode / 255.0f;

	return {red, green, blue};
}

void Color::PrintRGB() const
{
	std::cout << "Red: " << m_Red << ", ";
	std::cout << "Green: " << m_Green << ", ";
	std::cout << "Blue: " << m_Blue << std::endl;
}
