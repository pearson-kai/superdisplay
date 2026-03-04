#ifndef MESH_H
#define MESH_H

#include <vector>
#include "Triangle.hpp"

class Mesh
{
    private:
        std::vector<Triangle> triangles;
        Vec3 pos; // world position
        Vec3 scale; // scale (baseline is (1,1,1))
        Vec3 rotation; // rotation is in degrees
    public:
        Mesh(Vec3 _pos = Vec3(0,0,0, Colour()), Vec3 _scale = Vec3(1,1,1, Colour()), Vec3 _rotation = Vec3(0,0,0, Colour()));

        int numTri(); // returns number of triangles in vector
        Triangle getTri(int index); // returns triangle at index in vector

        void add(Triangle _tri);
        Triangle remove(int index = 0);

        Vec3 getPos();
        void setPos(int index, float value);
        Vec3 applyPos(Vec3 apply);

        Vec3 getScale();
        void setScale(int index, float value);
        Vec3 applyScale(Vec3 apply);
        
        Vec3 getRotation();
        void setRotation(int index, float value);
        Vec3 applyRotation(Vec3 apply);

        Vec3 localize(Vec3 apply);
};

#endif