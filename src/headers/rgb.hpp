#pragma once

#include <stdint.h>
#include <math.h>

class RGB {
public:
	RGB(uint8_t r, uint8_t g, uint8_t b) :
	r(r), g(g), b(b), a(255)
	{
	}
	RGB() = default;

	static RGB HSVtoRGB(float H, float S,float V);
	static RGB hexToRGB(uint32_t hex);

public:
    uint8_t a;
    uint8_t b;
    uint8_t g;
    uint8_t r;
};