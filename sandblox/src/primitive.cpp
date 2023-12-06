#include "primitive.h"

Primitive::Primitive() {}

std::vector<float> transformVertices(std::vector<GLfloat>& vertices, glm::mat4& matrix) {
    std::vector<GLfloat> transformedVertices;
    for (int i = 0; i < vertices.size(); i+=8) {
        glm::vec4 vector(vertices[i + 0], vertices[i + 1], vertices[i + 2], 1);
        vector = vector * matrix;
        transformedVertices.push_back(vector[0]);
        transformedVertices.push_back(vector[1]);
        transformedVertices.push_back(vector[2]);
        transformedVertices.push_back(vertices[i + 3]);
        transformedVertices.push_back(vertices[i + 4]);
        transformedVertices.push_back(vertices[i + 5]);
        transformedVertices.push_back(vertices[i + 6]);
        transformedVertices.push_back(vertices[i + 7]);
    }
    return transformedVertices;
}

void Primitive::drawShape(GLuint& shader) {
    glGenBuffers(1, &vbo);
    glGenVertexArrays(1, &vao);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, m_vertexData.size() * sizeof(GLfloat), m_vertexData.data(), GL_STATIC_DRAW);

    glBindVertexArray(vao);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(GLfloat), reinterpret_cast<void*>(0));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(GLfloat), reinterpret_cast<void*>(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 9 * sizeof(GLfloat), reinterpret_cast<void*>(6 * sizeof(GLfloat)));
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, 9 * sizeof(GLfloat), reinterpret_cast<void*>(8 * sizeof(GLfloat)));

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}
