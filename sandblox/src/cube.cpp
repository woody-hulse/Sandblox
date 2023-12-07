#include "Cube.h"

void Cube::updateParams(uint8_t blockType) {
    m_vertexData = std::vector<float>();
    m_param1 = 1;
    setVertexData(blockType + 1);
}

void Cube::makeTile(glm::vec3 topLeft,
                    glm::vec3 topRight,
                    glm::vec3 bottomLeft,
                    glm::vec3 bottomRight) {

    glm::vec3 normal1 = -glm::normalize(glm::cross(topLeft - topRight, topLeft - bottomLeft));
    glm::vec3 normal2 = -glm::normalize(glm::cross(bottomRight - bottomLeft, bottomRight - topRight));

    insertVec3(m_vertexData, topLeft);
    insertVec3(m_vertexData, normal1);
    insertVec3(m_vertexData, bottomLeft);
    insertVec3(m_vertexData, normal1);
    insertVec3(m_vertexData, topRight);
    insertVec3(m_vertexData, normal1);

    insertVec3(m_vertexData, topRight);
    insertVec3(m_vertexData, normal2);
    insertVec3(m_vertexData, bottomLeft);
    insertVec3(m_vertexData, normal2);
    insertVec3(m_vertexData, bottomRight);
    insertVec3(m_vertexData, normal2);
}

void Cube::makeFace(glm::vec3 topLeft,
                       glm::vec3 topRight,
                       glm::vec3 bottomLeft,
                       glm::vec3 bottomRight,
                    uint8_t blockType) {

    glm::vec3 normal1 = -glm::normalize(glm::cross(topLeft - topRight, topLeft - bottomLeft));
    glm::vec3 normal2 = -glm::normalize(glm::cross(bottomRight - bottomLeft, bottomRight - topRight));

    insertVec3(m_vertexData, topLeft);
    insertVec3(m_vertexData, normal1);
    insertVec2(m_vertexData, glm::vec2(0.f, 1.f));
    m_vertexData.push_back(blockType);
    insertVec3(m_vertexData, bottomLeft);
    insertVec3(m_vertexData, normal1);
    insertVec2(m_vertexData, glm::vec2(0.f, 0.f));
    m_vertexData.push_back(blockType);
    insertVec3(m_vertexData, topRight);
    insertVec3(m_vertexData, normal1);
    insertVec2(m_vertexData, glm::vec2(1.f, 1.f));
    m_vertexData.push_back(blockType);

    insertVec3(m_vertexData, topRight);
    insertVec3(m_vertexData, normal2);
    insertVec2(m_vertexData, glm::vec2(1.f, 1.f));
    m_vertexData.push_back(blockType);
    insertVec3(m_vertexData, bottomLeft);
    insertVec3(m_vertexData, normal2);
    insertVec2(m_vertexData, glm::vec2(0.f, 0.f));
    m_vertexData.push_back(blockType);
    insertVec3(m_vertexData, bottomRight);
    insertVec3(m_vertexData, normal2);
    insertVec2(m_vertexData, glm::vec2(1.f, 0.f));
    m_vertexData.push_back(blockType);
}

void Cube::setVertexData(uint8_t blockType) {

    makeFace(glm::vec3(-0.5f,  0.5f, 0.5f),
             glm::vec3( 0.5f,  0.5f, 0.5f),
             glm::vec3(-0.5f, -0.5f, 0.5f),
             glm::vec3( 0.5f, -0.5f, 0.5f),
             blockType);

    makeFace(glm::vec3( 0.5f,  0.5f, -0.5f),
             glm::vec3(-0.5f,  0.5f, -0.5f),
             glm::vec3( 0.5f, -0.5f, -0.5f),
             glm::vec3(-0.5f, -0.5f, -0.5f),
             blockType);

    makeFace(glm::vec3( 0.5f,  0.5f,  0.5f),
             glm::vec3( 0.5f,  0.5f, -0.5f),
             glm::vec3( 0.5f, -0.5f,  0.5f),
             glm::vec3( 0.5f, -0.5f, -0.5f),
             blockType);

    makeFace(glm::vec3(-0.5f,  0.5f, -0.5f),
             glm::vec3(-0.5f,  0.5f,  0.5f),
             glm::vec3(-0.5f, -0.5f, -0.5f),
             glm::vec3(-0.5f, -0.5f,  0.5f),
             blockType);

    makeFace(glm::vec3(-0.5f,  0.5f, -0.5f),
             glm::vec3( 0.5f,  0.5f, -0.5f),
             glm::vec3(-0.5f,  0.5f,  0.5f),
             glm::vec3( 0.5f,  0.5f,  0.5f),
             blockType);

    makeFace(glm::vec3(-0.5f, -0.5f, 0.5f),
             glm::vec3( 0.5f, -0.5f, 0.5f),
             glm::vec3(-0.5f, -0.5f, -0.5f),
             glm::vec3( 0.5f, -0.5f, -0.5f),
             blockType);
}

void Cube::insertVec3(std::vector<float> &data, glm::vec3 v) {
    data.push_back(v.x);
    data.push_back(v.y);
    data.push_back(v.z);
}

void Cube::insertVec2(std::vector<float> &data, glm::vec2 v) {
    data.push_back(v.x);
    data.push_back(v.y);
}
