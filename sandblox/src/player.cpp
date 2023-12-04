#include "player.h"


Player::Player() {}

Player::Player(Terrain* terrain, Camera* camera) {
    this->terrain = terrain;
    this->camera = camera;
}

void Player::move(glm::vec3 direction) {
    float epsilon = 0.001f;
    if (!collisionDetect(glm::vec3(direction.x * moveSpeed, 0.f, 0.f)))
        camera->data->pos.x += direction.x * moveSpeed;
    else velocity.x = 0;
    if (!collisionDetect(glm::vec3(0.f, direction.y * moveSpeed, 0.f))) {
        camera->data->pos.y += direction.y * moveSpeed;
        grounded = false;
    } else {
        grounded = true;
        velocity.y = 0;
    }
    if (!collisionDetect(glm::vec3(0.f, 0.f, direction.z * moveSpeed)))
        camera->data->pos.z += direction.z * moveSpeed;
    else velocity.z = 0;
}

void Player::simulate(float deltaTime) {
    if (gravity > 0.f) {
        velocity.y -= gravity * deltaTime;
    }

    if (grounded) {
        velocity.x *= 0.9f;
        velocity.z *= 0.9f;
    }

    move(velocity);
}


bool Player::collisionDetect(glm::vec3 direction) {
    std::vector<glm::vec3> corners = {
        glm::vec3(camera->data->pos) + glm::vec3(0.5f, 0.5f, 0.5f),
        glm::vec3(camera->data->pos) + glm::vec3(-0.5f, 0.5f, 0.5f),
        glm::vec3(camera->data->pos) + glm::vec3(0.5f, 0.5f, -0.5f),
        glm::vec3(camera->data->pos) + glm::vec3(-0.5f, 0.5f, -0.5f),

        glm::vec3(camera->data->pos) + glm::vec3(0.5f, -1.5f, 0.5f),
        glm::vec3(camera->data->pos) + glm::vec3(-0.5f, -1.5f, 0.5f),
        glm::vec3(camera->data->pos) + glm::vec3(0.5f, -1.5f, -0.5f),
        glm::vec3(camera->data->pos) + glm::vec3(-0.5f, -1.5f, -0.5f)
    };

    bool collision = false;
    for (glm::vec3 corner : corners) {
        corner += direction;
        int x = floor(corner.x);
        int z = floor(corner.y);
        int y = floor(corner.z);

        if (x >= 0 && x < terrain->sizeX) {
            if (y >= 0 && z < terrain->sizeY) {
                if (z >= 0 && z < terrain->sizeZ) {
                    if (terrain->terrain[x][y][z] == 1)
                        collision = true;
                }
            }
        }
    }

    return collision;
}
