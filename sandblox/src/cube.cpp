#include "Cube.h"

void Cube::updateParams(int param1, int param2) {
    m_vertexData = std::vector<float>();
    m_param1 = 1;
    setVertexData();
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
                    glm::vec3 bottomRight) {

    float inc = 1.f / m_param1;
    for (int i = 0; i < m_param1; i++) {
        for (int j = 0; j < m_param1; j++) {
            float x = (float)i / m_param1;
            float y = (float)j / m_param1;
            glm::vec3 tl = (topLeft - bottomLeft) * x + bottomRight;
            glm::vec3 tr = (topRight - bottomRight) * x + bottomRight;
            glm::vec3 bl = (topLeft - bottomLeft) * (x + inc) + bottomRight;
            glm::vec3 br = (topRight - bottomRight) * (x + inc) + bottomRight;

            tl += (topLeft - topRight) * y;
            tr += (topLeft - topRight) * (y + inc);
            bl += (bottomLeft - bottomRight) * y;
            br += (bottomLeft - bottomRight) * (y + inc);

            makeTile(tl, tr, bl, br);
        }
    }


}

void Cube::setVertexData() {

    makeFace(glm::vec3(-0.5f,  0.5f, 0.5f),
             glm::vec3( 0.5f,  0.5f, 0.5f),
             glm::vec3(-0.5f, -0.5f, 0.5f),
             glm::vec3( 0.5f, -0.5f, 0.5f));

    makeFace(glm::vec3( 0.5f,  0.5f, -0.5f),
             glm::vec3(-0.5f,  0.5f, -0.5f),
             glm::vec3( 0.5f, -0.5f, -0.5f),
             glm::vec3(-0.5f, -0.5f, -0.5f));

    makeFace(glm::vec3( 0.5f,  0.5f,  0.5f),
             glm::vec3( 0.5f,  0.5f, -0.5f),
             glm::vec3( 0.5f, -0.5f,  0.5f),
             glm::vec3( 0.5f, -0.5f, -0.5f));

    makeFace(glm::vec3(-0.5f,  0.5f, -0.5f),
             glm::vec3(-0.5f,  0.5f,  0.5f),
             glm::vec3(-0.5f, -0.5f, -0.5f),
             glm::vec3(-0.5f, -0.5f,  0.5f));

    makeFace(glm::vec3(-0.5f,  0.5f, -0.5f),
             glm::vec3( 0.5f,  0.5f, -0.5f),
             glm::vec3(-0.5f,  0.5f,  0.5f),
             glm::vec3( 0.5f,  0.5f,  0.5f));

    makeFace(glm::vec3(-0.5f, -0.5f, 0.5f),
             glm::vec3( 0.5f, -0.5f, 0.5f),
             glm::vec3(-0.5f, -0.5f, -0.5f),
             glm::vec3( 0.5f, -0.5f, -0.5f));
}

void Cube::insertVec3(std::vector<float> &data, glm::vec3 v) {
    data.push_back(v.x);
    data.push_back(v.y);
    data.push_back(v.z);
}
