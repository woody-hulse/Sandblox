#ifndef PLAYER_H
#define PLAYER_H

#include "terrain.h"
#include "camera.h"
#include <map>

enum GameMode {
    ADVENTURE,
    CREATIVE
};

class Player
{
public:
    Player();
    Player(Terrain* terrain, Camera* camera);
    void move(glm::vec3 direction);
    void simulate(float deltaTime);
    bool collisionDetect(glm::vec3 direction);
    void rectifyPlayer();

    glm::vec3 velocity = glm::vec3(0.f);
    float moveSpeed = 10.0f;
    float gravity = 0.05f;
    bool grounded = false;

    Terrain* terrain;
    Camera* camera;

    GameMode gameMode = GameMode::ADVENTURE;
    int inventorySelection = 0;
    int inventory[8] = {0, 0, 0, 0, 200, 0, 0, 0};
};

#endif // PLAYER_H
