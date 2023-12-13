#pragma once

#include <vector>
#include <glm/glm.hpp>
#include <GL/glew.h>

#include <iostream>

class Primitive
{
public:
    Primitive();

    virtual void updateParams(uint8_t blockType) = 0;
    virtual std::vector<float> generateShape() = 0;
    int vertexSize() const { return m_vertexData.size(); }

    void drawShape(GLuint& shader);

    GLuint vbo;
    GLuint vao;
    std::vector<float> m_vertexData;

protected:
    int m_param1;
    int m_param2;
};
