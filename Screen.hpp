#ifndef SCREEN_H
#define SCREEN_H

#include <string>

#include "SDL3/SDL.h"
#include "Camera.hpp"

class Screen
{
    private:
        std::string name;
        int width;
        int height;
        SDL_Renderer* renderer;
        SDL_Window* window;
        SDL_Texture* texture;

        std::vector<float> zBuffer;
        std::vector<uint32_t> frameBuffer;

        void clearBuffer();
        void setColour(Colour colour);
    public:
        Screen();
        Screen(int _width, int _height);
        Screen(std::string _name, int _width, int _height);

        void coatColour(Colour colour);

        void buildBuffer(Camera camera);
        void rasterizer(Triangle triangle);

        void printBuffer();
        void displayScene();
        void clearScene();

        void explode(); // deletes SDL3 window and renderer safely
};

#endif