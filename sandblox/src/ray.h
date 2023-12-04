#ifndef RAY_H
#define RAY_H

#include "terrain.h"
#include "camera.h"
#include <glm/gtx/string_cast.hpp>

struct Ray {
    glm::vec4 origin;
    glm::vec4 direction;
};

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
};

#endif // RAY_H
