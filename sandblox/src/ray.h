#ifndef RAY_H
#define RAY_H

#include "terrain.h"
#include "camera.h"
#include <glm/gtx/string_cast.hpp>

class RayCast
{
public:
    RayCast();
    RayCast(Terrain* terrain, Camera* camera);
    void computeRay(glm::vec2 mousePosition, int width, int height);
    IntersectData intersectRay();

    Terrain* terrain;
    Camera* camera;

    Ray ray;

    float intersectDistance = 10;
};

#endif // RAY_H
