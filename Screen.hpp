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

        void clearBuffer();
        void setColour(Colour colour);
    public:
        
        std::vector<uint32_t> frameBuffer;
        Screen(std::string _name = "superdisplay", int _width = 800, int _height = 600);

        void coatColour(Colour colour);

        void buildBuffer(Camera camera);
        void rasterizer(Vec3& v0, Vec3& v1, Vec3& v2);

        void resizeBuffer();
        void printBuffer();
        void displayScene();
        void clearScene();

        void explode(); // deletes SDL3 window and renderer safely
};

#endif