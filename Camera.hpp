#ifndef CAMERA_H
#define CAMERA_H

#include "Vec3.hpp"
#include "World.hpp"

class Camera
{
    private:
        World* world;
        Vec3 pos;
        Vec3 forward;
        Vec3 up;
        Vec3 right;
        bool forwardLock;
    public:
        World* getWorld();
        void setWorld(World* _world);

        Vec3 getPos();
        void setPos(int index, float value);

        Vec3 getForward();
        void setForward(int index, float value);
        
        Vec3 getUp();
        void setUp(int index, float value);

        void normalizeForward();
        void normalizeRight();
        void updateRight();
};

#endif