#ifndef SCREEN_H
#define SCREEN_H

#include <string>
#include <filesystem>
#include <fstream>
#include <cmath>

#include "SDL3/SDL.h"
#include "SDL3/SDL_gpu.h"
#include "Camera.hpp"

const int DIST_CONST = 200;
const int NEAR_PLANE = 0.001; // distance at which near plane clipping starts being calculated

class Screen
{
    private:
        std::string name;
        int widthScreen;
        int heightScreen;
        int widthResolution;
        int heightResolution;
        SDL_Renderer* renderer;
        SDL_Window* window;
        SDL_Texture* texture;

        std::vector<float> zBuffer;

        void clearBuffer();
        void setColour(Colour colour);
    public:
        
        std::vector<uint32_t> frameBuffer;
        Screen(std::string _name = "superdisplay", int _width = 800, int _height = 600);


        static void print(std::string myString);

        int getWidth();
        int getHeight();

        
        void coatColour(Colour colour);

        void resizeBuffer();

        std::vector<Triangle> clipTriangleNearPlane(Vec3 p1,Vec3 p2,Vec3 p3,float nearPlane);
        void buildBuffer(Camera& camera);
        void rasterizer(const Vec3& v0, const Vec3& v1, const Vec3& v2);

        void displaySplash(SDL_Surface* _splash);
        void printBuffer();
        
        void explode(); // deletes SDL3 window and renderer safely
};

#endif