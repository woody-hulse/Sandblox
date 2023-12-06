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
    if (gravity > 0.f && gameMode == GameMode::ADVENTURE) {
        velocity.y -= gravity * deltaTime;
    }

    if (grounded && gameMode == GameMode::ADVENTURE) {
        velocity.x *= 0.5f;
        velocity.z *= 0.5f;
    } else {
        velocity.x *= 0.95f;
        velocity.z *= 0.95f;
    }

    move(velocity);
}


bool Player::collisionDetect(glm::vec3 direction) {
    float scale = 0.9f;
    std::vector<glm::vec3> corners = {
        glm::vec3(camera->data->pos) + glm::vec3(0.5f, 0.0f, 0.5f) * scale,
        glm::vec3(camera->data->pos) + glm::vec3(-0.5f, 0.0f, 0.5f) * scale,
        glm::vec3(camera->data->pos) + glm::vec3(0.5f, 0.0f, -0.5f) * scale,
        glm::vec3(camera->data->pos) + glm::vec3(-0.5f, 0.0f, -0.5f) * scale,

        glm::vec3(camera->data->pos) + glm::vec3(0.5f, -0.95f, 0.5f) * scale,
        glm::vec3(camera->data->pos) + glm::vec3(-0.5f, -0.95f, 0.5f) * scale,
        glm::vec3(camera->data->pos) + glm::vec3(0.5f, -0.95f, -0.5f) * scale,
        glm::vec3(camera->data->pos) + glm::vec3(-0.5f, -0.95f, -0.5f) * scale,

        glm::vec3(camera->data->pos) + glm::vec3(0.5f, -1.9f, 0.5f) * scale,
        glm::vec3(camera->data->pos) + glm::vec3(-0.5f, -1.9f, 0.5f) * scale,
        glm::vec3(camera->data->pos) + glm::vec3(0.5f, -1.9f, -0.5f) * scale,
        glm::vec3(camera->data->pos) + glm::vec3(-0.5f, -1.9f, -0.5f) * scale
    };

    bool collision = false;
    for (glm::vec3 corner : corners) {
        corner += direction;
        int x = floor(corner.x + 0.5f);
        int z = floor(corner.y + 0.5f);
        int y = floor(corner.z + 0.5f);

        if (x >= 0 && x < terrain->sizeX) {
            if (y >= 0 && z < terrain->sizeY) {
                if (z >= 0 && z < terrain->sizeZ) {
                    if (terrain->terrain[x][y][z] > 0)
                        collision = true;
                }
            }
        }
    }

    return collision;
}

void Player::rectifyPlayer() {
    while (collisionDetect(glm::vec3(0.f))) {
        camera->data->pos.y += 0.3f;
    }
}
