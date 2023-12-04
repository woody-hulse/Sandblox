#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>
#include "utils/scenedata.h"

class Camera {
public:
    Camera();
    Camera(SceneCameraData* data, int width, int height);

    glm::mat4 getViewMatrix() const { return viewMatrix; }
    glm::mat4 getInverseViewMatrix() const { return inverseViewMatrix; }

    glm::mat4 getPerspectiveMatrix() const { return perspectiveMatrix; }
    glm::mat4 getInversePerspectiveMatrix() const { return inversePerspectiveMatrix; }

    void computeViewMatrix();
    void computePerspectiveMatrix();

    void setAspectRatio(int width, int height);
    float getAspectRatio() const;

    float getHeightAngle() const;

    float getFocalLength() const;

    float getAperture() const;

    SceneCameraData* data;

private:
    glm::mat4 viewMatrix;
    glm::mat4 inverseViewMatrix;

    glm::mat4 perspectiveMatrix;
    glm::mat4 inversePerspectiveMatrix;

    float aspectRatio;
};


#endif // CAMERA_H
