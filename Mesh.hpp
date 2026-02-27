#ifndef MESH_H
#define MESH_H

#include <vector>
#include "Triangle.hpp"

class Mesh
{
    private:
        std::vector<Triangle> triangles;
        Vec3 pos;
        Vec3 scale;
        Vec3 rotation;
    public:
        Mesh();

        int numTri(); // returns number of triangles in vector
        Triangle getTri(int index); // returns triangle at index in vector

        void add(int index = 0, Triangle _tri);
        Triangle remove(int index = 0);

        Vec3 getPos();
        void setPos(int index, float value);

        Vec3 getScale();
        void setScale(int index, float value);
        
        Vec3 getRotation();
        void setRotation(int index, float value);
};

#endif