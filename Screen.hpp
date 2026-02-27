#ifndef SCREEN_H
#define SCREEN_H

#include "SDL3/SDL.h"
#include "Camera.hpp"

class Screen
{
    private:
        int width;
        int height;
        SDL_Renderer* renderer;
        SDL_Window* window;
    public:
        void buildScene(Camera camera);
        void displayScene();
        void clearScene();

        void explode(); // deletes SDL3 window and renderer safely
};

#endif