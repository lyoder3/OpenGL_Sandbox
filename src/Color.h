#pragma once
#include<string>
#include <sstream>

struct RGBColor
{
	float red;
	float green;
	float blue;
};

class Color
{
    private:
		float m_Red;
		float m_Green;
		float m_Blue;
		unsigned int m_HexCode;
    public:
		Color(RGBColor col);
		Color(unsigned int hexCode);
		~Color();

		RGBColor GetRGB() const;
		std::stringstream GetHexString() const;

		void PrintRGB() const;
	private:
		unsigned int ConvertRGBToHex(RGBColor col) const;
		RGBColor ConvertHexToRGB(unsigned int hexCode) const;
};