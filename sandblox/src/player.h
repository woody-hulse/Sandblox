#ifndef PLAYER_H
#define PLAYER_H

#include "terrain.h"
#include "camera.h"

class Player
{
public:
    Player();
    Player(Terrain* terrain, Camera* camera);
    void move(glm::vec3 direction);
    void simulate(float deltaTime);
    bool collisionDetect(glm::vec3 direction);

    glm::vec3 velocity = glm::vec3(0.f);
    float moveSpeed = 20.0f;
    float gravity = 0.02f;
    bool grounded = false;

    Terrain* terrain;
    Camera* camera;
};

#endif // PLAYER_H
