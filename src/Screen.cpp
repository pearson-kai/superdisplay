#include <cmath>
#include <algorithm>
#include <cstring>
#include "Screen.hpp"

void Screen::print(std::string myString)
{
    std::cout << myString << std::endl;
}

Screen::Screen(std::string _name, int _widthScreen, int _heightScreen)
{
    name = _name;
    widthScreen = _widthScreen;
    heightScreen = _heightScreen;
    widthResolution = 800;
    heightResolution = 600;

    window = SDL_CreateWindow(_name.c_str(), _widthScreen, _heightScreen, SDL_WINDOW_RESIZABLE);
    if (!window)
    {
        std::cerr << "SDL_CreateWindow failed: " << SDL_GetError() << std::endl;
    }

    renderer = SDL_CreateRenderer(window, NULL);
    if (!renderer)
    {
        std::cerr << "SDL_CreateRenderer failed: " << SDL_GetError() << std::endl;
    }

    texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, _widthScreen, _heightScreen);
    if (!texture)
    {
        std::cerr << "SDL_CreateTexture failed: " << SDL_GetError() << std::endl;
    }

    resizeBuffer();
}

int Screen::getWidth()
{
    return widthScreen;
}
int Screen::getHeight()
{
    return heightScreen;
}

void Screen::displaySplash(SDL_Surface* _splash)
{
    memcpy(&frameBuffer[0], _splash->pixels, widthScreen * heightScreen * 4);
}

void Screen::setColour(Colour colour)
{
    SDL_SetRenderDrawColor(renderer, colour.get(0),colour.get(1),colour.get(2),colour.get(3));
}

void Screen::clearBuffer()
{
    std::fill(zBuffer.begin(), zBuffer.end(), -1);
    std::fill(frameBuffer.begin(),frameBuffer.end(),0xFF000000);
}

void Screen::resizeBuffer()
{
    zBuffer.resize(widthScreen * heightScreen);
    frameBuffer.resize(widthScreen * heightScreen);
    clearBuffer();
}

std::vector<Triangle> Screen::clipTriangleNearPlane(Vec3 p1,Vec3 p2,Vec3 p3,float nearPlane)
{
    std::vector<Triangle> result;

    bool in1 = p1.get(2) >= nearPlane;
    bool in2 = p2.get(2) >= nearPlane;
    bool in3 = p3.get(2) >= nearPlane;

    int insideCount = in1 + in2 + in3;

    if (insideCount == 0)
    {
        return result;
    }

    if (insideCount == 3)
    {
        result.push_back({p1, p2, p3});
        return result;
    }

    auto intersect =
    [&](const Vec3& inside, const Vec3& outside)
    {
        float t = (nearPlane - inside.get(2)) / (outside.get(2) - inside.get(2));

        return Vec3(inside.get(0) + t * (outside.get(0) - inside.get(0)),inside.get(1) + t * (outside.get(1) - inside.get(1)),nearPlane);
    };

    if (insideCount == 1)
    {
        Vec3 inside;
        Vec3 out1;
        Vec3 out2;

        if (in1)
        {
            inside = p1;
            out1 = p2;
            out2 = p3;
        }
        else if (in2)
        {
            inside = p2;
            out1 = p1;
            out2 = p3;
        }
        else
        {
            inside = p3;
            out1 = p1;
            out2 = p2;
        }

        Vec3 i1 = intersect(inside, out1);
        Vec3 i2 = intersect(inside, out2);

        result.push_back(Triangle(inside, i1, i2));
    }

    if (insideCount == 2)
    {
        Vec3 inside1;
        Vec3 inside2;
        Vec3 outside;

        if (!in1)
        {
            outside = p1;
            inside1 = p2;
            inside2 = p3;
        }
        else if (!in2)
        {
            outside = p2;
            inside1 = p1;
            inside2 = p3;
        }
        else
        {
            outside = p3;
            inside1 = p1;
            inside2 = p2;
        }

        Vec3 i1 = intersect(inside1, outside);
        Vec3 i2 = intersect(inside2, outside);

        result.push_back(Triangle(inside1, inside2, i2));
        result.push_back(Triangle(inside1, i2, i1));
    }

    return result;
}

void Screen::buildBuffer(Camera& camera)
{
    clearBuffer();

    World* world = camera.getWorld();
    if (!world)
    {
        return;
    }

    int numMeshes = world->numObj(true);
    if (numMeshes == 0)
    {
        return;
    }

    bool cull = camera.getCulling();

    camera.normalizeForward();
    camera.normalizeUp();
    camera.updateRight();
    camera.normalizeRight();

    const Vec3 camPos = camera.getPos();
    const Vec3 camForward = camera.getForward();
    const Vec3 camUp = camera.getUp();
    const Vec3 camRight = camera.getRight();

    const float halfWidth = widthResolution * 0.5f;
    const float halfHeight = heightResolution * 0.5f;
    const float rad = camera.getFov() * std::acos(-1) / 180.0f;
    const float fval = halfHeight / std::tan(rad * 0.5f);

    for (int i = 0; i < numMeshes; ++i)
    {
        Mesh* currentMesh = world->getObj(true, i);
        int numTriangles = currentMesh->numTri();

        for (int j = 0; j < numTriangles; ++j)
        {
            const Triangle currentTri = currentMesh->getTri(j);

            Vec3 p1 = currentMesh->localize(currentTri.get(0));
            Vec3 p2 = currentMesh->localize(currentTri.get(1));
            Vec3 p3 = currentMesh->localize(currentTri.get(2));

            p1 = p1.subtract(camPos).applyRotation(camForward, camUp, camRight);
            p2 = p2.subtract(camPos).applyRotation(camForward, camUp, camRight);
            p3 = p3.subtract(camPos).applyRotation(camForward, camUp, camRight);

            if(cull)
            {
                Vec3 normal = p2.subtract(p1).cross(p3.subtract(p1));
                if (normal.get(2) < 0)
                {
                    continue;
                }
            }

            std::vector<Triangle> clipped = clipTriangleNearPlane(p1,p2,p3,NEAR_PLANE);

            for (const Triangle& tri : clipped)
            {
                Vec3 cp1 = camera.project(fval, tri.get(0));
                Vec3 cp2 = camera.project(fval, tri.get(1));
                Vec3 cp3 = camera.project(fval, tri.get(2));

                cp1.set(0, cp1.get(0) + halfWidth);
                cp1.set(1, halfHeight - cp1.get(1));

                cp2.set(0, cp2.get(0) + halfWidth);
                cp2.set(1, halfHeight - cp2.get(1));

                cp3.set(0, cp3.get(0) + halfWidth);
                cp3.set(1, halfHeight - cp3.get(1));

                rasterizer(cp1, cp2, cp3);
            }
        }
    }
}

void Screen::rasterizer(const Vec3& v0, const Vec3& v1, const Vec3& v2)
{
    int minX = std::max(0, (int)floor(std::min(v0.get(0), std::min(v1.get(0), v2.get(0)))));
    int maxX = std::min(widthScreen - 1, (int)ceil(std::max(v0.get(0), std::max(v1.get(0), v2.get(0)))));
    int minY = std::max(0, (int)floor(std::min(v0.get(1), std::min(v1.get(1), v2.get(1)))));
    int maxY = std::min(heightScreen - 1, (int)ceil(std::max(v0.get(1), std::max(v1.get(1), v2.get(1)))));

    if (minX > maxX || minY > maxY)
    {
        return;
    }

    float x0 = v0.get(0), y0 = v0.get(1), z0 = v0.get(2);
    float x1 = v1.get(0), y1 = v1.get(1), z1 = v1.get(2);
    float x2 = v2.get(0), y2 = v2.get(1), z2 = v2.get(2);

    const Colour c0 = v0.getColour();
    const Colour c1 = v1.getColour();
    const Colour c2 = v2.getColour();

    const float cr0 = static_cast<float>(c0.get(0)), cg0 = static_cast<float>(c0.get(1)), cb0 = static_cast<float>(c0.get(2));
    const float cr1 = static_cast<float>(c1.get(0)), cg1 = static_cast<float>(c1.get(1)), cb1 = static_cast<float>(c1.get(2));
    const float cr2 = static_cast<float>(c2.get(0)), cg2 = static_cast<float>(c2.get(1)), cb2 = static_cast<float>(c2.get(2));

    float A0 = y1 - y2;
    float B0 = x2 - x1;
    float C0 = x1 * y2 - x2 * y1;

    float A1 = y2 - y0;
    float B1 = x0 - x2;
    float C1 = x2 * y0 - x0 * y2;

    float A2 = y0 - y1;
    float B2 = x1 - x0;
    float C2 = x0 * y1 - x1 * y0;

    float area = A2 * x2 + B2 * y2 + C2;
    if (area == 0.0f)
    {
        return;
    }
    float invArea = 1.0f / area;

    float startX = minX + 0.5f;
    float startY = minY + 0.5f;

    float w0_row = A0 * startX + B0 * startY + C0;
    float w1_row = A1 * startX + B1 * startY + C1;
    float w2_row = A2 * startX + B2 * startY + C2;

    uint32_t* framePtr = frameBuffer.data();
    float* depthPtr = zBuffer.data();
    
    for (int y = minY; y <= maxY; y++)
    {
        float w0 = w0_row;
        float w1 = w1_row;
        float w2 = w2_row;

        int index = y * widthScreen + minX;

        for (int x = minX; x <= maxX; x++)
        {
            if ((w0 <= 0 && w1 <= 0 && w2 <= 0) || (w0 >= 0 && w1 >= 0 && w2 >= 0))
            {
                float b0 = w0 * invArea;
                float b1 = w1 * invArea;
                float b2 = w2 * invArea;

                float depth = b0 * z0 + b1 * z1 + b2 * z2;

                if (depthPtr[index] < 0.0f || depth < depthPtr[index])
                {
                    depthPtr[index] = depth;

                    uint8_t r = static_cast<uint8_t>(std::clamp(b0 * cr0 + b1 * cr1 + b2 * cr2, 0.0f, 255.0f));
                    uint8_t g = static_cast<uint8_t>(std::clamp(b0 * cg0 + b1 * cg1 + b2 * cg2, 0.0f, 255.0f));
                    uint8_t blue = static_cast<uint8_t>(std::clamp(b0 * cb0 + b1 * cb1 + b2 * cb2, 0.0f, 255.0f));
                    framePtr[index] = (255u << 24) | (static_cast<uint32_t>(r) << 16) | (static_cast<uint32_t>(g) << 8) | static_cast<uint32_t>(blue);
                }
            }

            w0 += A0;
            w1 += A1;
            w2 += A2;

            index++;
        }

        w0_row += B0;
        w1_row += B1;
        w2_row += B2;
    }
}

void Screen::printBuffer()
{
    // paste buffer to texture and upload to renderer
    SDL_RenderClear(renderer);

    if (!SDL_UpdateTexture(texture, nullptr, frameBuffer.data(), widthScreen * sizeof(uint32_t)))
    {
        std::cerr << "SDL_UpdateTexture failed: " << SDL_GetError() << std::endl;
        return;
    }

    if (!SDL_RenderTexture(renderer, texture, nullptr, nullptr))
    {
        std::cerr << "SDL_RenderTexture failed: " << SDL_GetError() << std::endl;
        return;
    }

    SDL_RenderPresent(renderer);
}


void Screen::coatColour(Colour colour)
{
    setColour(colour);
    SDL_RenderClear(renderer);
    SDL_RenderPresent(renderer);
    setColour(Colour(0,0,0,255));

    return;
}

void Screen::explode()
{
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_DestroyTexture(texture);

    texture = nullptr;
    renderer = nullptr;
    window = nullptr;
    
    return;
}