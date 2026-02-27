#ifndef VEC3_H
#define VEC3_H

#include <cstdint>
#include "Colour.hpp"

class Vec3
{
    private:
        float values[3];
        Colour colour;
    public:
        Vec3(float _x = 0, float _y = 0, float _z = 0, Colour _colour = Colour());
        float get(int index);
        void set(int index, float value);

        Colour getColour();
        void setColour(Colour);
};

#endif