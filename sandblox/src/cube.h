#pragma once

#include "primitive.h"

class Cube : public Primitive
{
public:
    void updateParams(uint8_t blockType) override;
    std::vector<float> generateShape() override { return m_vertexData; }

private:
    void insertVec3(std::vector<float> &data, glm::vec3 v);
    void setVertexData(uint8_t blockType);
    void makeTile(glm::vec3 topLeft,
                  glm::vec3 topRight,
                  glm::vec3 bottomLeft,
                  glm::vec3 bottomRight);
    void makeFace(glm::vec3 topLeft,
                  glm::vec3 topRight,
                  glm::vec3 bottomLeft,
                  glm::vec3 bottomRight,
                  uint8_t blockType);
    void insertVec2(std::vector<float> &data, glm::vec2 v);
};
