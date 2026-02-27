#ifndef COLOUR_H
#define COLOUR_H

#include <cstdint>

class Colour
{
    private: 
        uint8_t colour[4];
    public:
        Colour(uint8_t _r = 0, uint8_t _g = 0, uint8_t _b = 0, uint8_t _a = 255);
        uint8_t get(int index);
};

#endif