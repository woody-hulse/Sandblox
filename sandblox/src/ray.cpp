#include "ray.h"
#include <glm/gtx/transform.hpp>

RayCast::RayCast() {};

RayCast::RayCast(Terrain* terrain, Camera* camera)
{
    this->terrain = terrain;
    this->camera = camera;

    computeRay(glm::vec2(0.5f, 0.5f), 800, 800);
}


void RayCast::computeRay(glm::vec2 mousePosition, int width, int height) {
    // if (camera == nullptr || terrain == nullptr) return;

    float V = 2 * tan(camera->data->heightAngle / 2.f);
    float U = V * camera->aspectRatio;

    float x = ((mousePosition.x + 0.5) / width - 0.5) * U;
    float y = ((height - mousePosition.y - 0.5) / height - 0.5) * V;
    float z = -1;

    glm::vec4 eye = camera->getInverseViewMatrix() * glm::vec4(0, 0, 0, 1);
    glm::vec4 pixel = camera->getInverseViewMatrix() * glm::vec4(x, y, z, 1);
    glm::vec4 dHat = glm::normalize(pixel - eye);
    ray.origin = eye;
    ray.direction = dHat;
}



IntersectData RayCast::intersectRay() {
    float epsilon = 0.0001;
    float min_t = -1;
    float min_face;
    IntersectData intersectData;
    intersectData.intersection = false;

    for (int x = 0; x < terrain->sizeX; x++) {
        for (int y = 0; y < terrain->sizeY; y++) {
            for (int z = 0; z < terrain->sizeZ; z++) {
                if (!terrain->rendered[x][y][z]) continue;
                float t = -1;
                int t_face = -1;
                int face = 0;

                glm::vec3 blockPosition(x, z, y);
                glm::mat4 inverseCtm = glm::mat4(glm::inverse(glm::translate(blockPosition)));
                Ray objectRay;
                objectRay.origin = inverseCtm * ray.origin;
                objectRay.direction = glm::normalize(inverseCtm * ray.direction);
                for (int i = 0; i < 3; i++) {
                    float t1 = (0.5 - objectRay.origin[i]) / objectRay.direction[i];
                    float t2 = (-0.5 - objectRay.origin[i]) / objectRay.direction[i];

                    glm::vec4 position1 = objectRay.origin + objectRay.direction * t1;
                    glm::vec4 position2 = objectRay.origin + objectRay.direction * t2;

                    if (abs(position1[(i + 1) % 3]) <= 0.5 &&
                        abs(position1[(i + 2) % 3]) <= 0.5) {
                        if (t1 > epsilon && (t1 < t || t == -1)) {
                            t = t1;
                            t_face = face;
                        }
                    }

                    if (abs(position2[(i + 1) % 3]) <= 0.5 &&
                        abs(position2[(i + 2) % 3]) <= 0.5) {
                        if (t2 > epsilon && (t2 < t || t == -1)) {
                            t = t2;
                            t_face = face + 1;
                        }
                    }

                    face += 2;
                }

                if (t >= 0) {
                    if (t < min_t || min_t == -1 && t < intersectDistance) {
                        min_t = t;
                        intersectData.x = x;
                        intersectData.y = y;
                        intersectData.z = z;
                        intersectData.face = t_face;
                        intersectData.intersection = true;
                    }
                }
            }
        }
    }

    return intersectData;
}
