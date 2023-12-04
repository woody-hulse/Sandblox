#include "camera.h"
#include "settings.h"
#include <iostream>

Camera::Camera(){}

Camera::Camera(SceneCameraData* data, int width, int height) {
    this->data = data;
    this->aspectRatio = (float)width / (float)height;

    computeViewMatrix();
    computePerspectiveMatrix();
}

void Camera::computeViewMatrix() {
    glm::vec3 w = -glm::normalize(data->look);
    glm::vec3 v = glm::normalize(glm::vec3(data->up) - glm::dot(glm::vec3(data->up), w) * w);
    glm::vec3 u = glm::cross(v, w);
    glm::vec3 p = glm::vec3(data->pos);

    glm::mat4 R(
        u.x, v.x, w.x, 0,
        u.y, v.y, w.y, 0,
        u.z, v.z, w.z, 0,
        0, 0, 0, 1
        );

    glm::mat4 T(
        1, 0, 0, 0,
        0, 1, 0, 0,
        0, 0, 1, 0,
        -p.x, -p.y, -p.z, 1
        );

    viewMatrix = R * T;
    inverseViewMatrix = glm::inverse(viewMatrix);
}

void Camera::computePerspectiveMatrix() {
    float nearPlane = 0.1f;
    float farPlane = 100.f;

    if (data == nullptr) return;
    glm::mat4 A(
        1, 0, 0, 0,
        0, 1, 0, 0,
        0, 0, -2, 0,
        0, 0, -1, 1
        );
    float c = -nearPlane / farPlane;
    glm::mat4 B(
        1, 0, 0, 0,
        0, 1, 0, 0,
        0, 0, 1.f / (1 + c), -1,
        0, 0, -c / (1 + c), 0
        );

    float V = tan(data->heightAngle / 2.f);
    float U = V * aspectRatio;

    glm::mat4 C(
        1 / (farPlane * U), 0, 0, 0,
        0, 1 / (farPlane * V), 0, 0,
        0, 0, 1 / farPlane, 0,
        0, 0, 0, 1
        );

    perspectiveMatrix = A * B * C * farPlane;
    inversePerspectiveMatrix = glm::inverse(perspectiveMatrix);
}

float Camera::getHeightAngle() const {
    return data->heightAngle;
}

void Camera::setAspectRatio(int width, int height) {
    aspectRatio = (float)width / (float)height;
    computePerspectiveMatrix();
}
