#include "ui.h"

UIOverlay::UIOverlay()
{

}

UIOverlay::UIOverlay(std::vector<GLfloat> data, GLint texture)
{
    this->texture = texture;

    for (int i = 0; i < data.size(); i++) {
        this->data.push_back(data[i]);
        if (i % 2 == 1) {
            this->data.push_back(0.f);
            this->data.push_back(0.f);
            this->data.push_back(0.f);
        }
    }

    init();
}

UIOverlay::UIOverlay(std::vector<std::vector<GLint>> data, GLint texture, float x, float y, float sizeX, float sizeY)
{
    this->texture = texture;

    for (int i = 0; i < data.size(); i++) {
        for (int j = 0; j < data[i].size(); j++) {
            if (data[i][j] == 0) continue;
            float dataX1 = (float)i / data.size();
            float dataY1 = (float)j / data[i].size();
            float dataX2 = (float)(i + 1) / data.size();
            float dataY2 = (float)(j + 1) / data[i].size();
            this->data.push_back(x + dataX1 * sizeX);
            this->data.push_back(y + dataY1 * sizeY);
            this->data.push_back(0.f);
            this->data.push_back(0.f);
            this->data.push_back(0.f);

            this->data.push_back(x + dataX2 * sizeX);
            this->data.push_back(y + dataY1 * sizeY);
            this->data.push_back(0.f);
            this->data.push_back(0.f);
            this->data.push_back(0.f);

            this->data.push_back(x + dataX1 * sizeX);
            this->data.push_back(y + dataY2 * sizeY);
            this->data.push_back(0.f);
            this->data.push_back(0.f);
            this->data.push_back(0.f);

            this->data.push_back(x + dataX2 * sizeX);
            this->data.push_back(y + dataY1 * sizeY);
            this->data.push_back(0.f);
            this->data.push_back(0.f);
            this->data.push_back(0.f);

            this->data.push_back(x + dataX2 * sizeX);
            this->data.push_back(y + dataY2 * sizeY);
            this->data.push_back(0.f);
            this->data.push_back(0.f);
            this->data.push_back(0.f);

            this->data.push_back(x + dataX1 * sizeX);
            this->data.push_back(y + dataY2 * sizeY);
            this->data.push_back(0.f);
            this->data.push_back(0.f);
            this->data.push_back(0.f);
        }
    }

    init();
}

void UIOverlay::init() {
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, data.size()*sizeof(GLfloat), data.data(), GL_STATIC_DRAW);
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    numVertices = data.size() / 5;

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), nullptr);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), reinterpret_cast<void*>(3 * sizeof(GLfloat)));

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void UIOverlay::translate(float x, float y, float sizeX, float sizeY) {
    for (int i = 0; i < data.size(); i++) {
        if (i % 5 == 0)
            data[i] = sizeX * data[i] + x;
        if (i % 5 == 1)
            data[i] = sizeY * data[i] + y;
    }

    init();
}

void UIOverlay::paint(GLuint program, int width, int height) {
    glUseProgram(program);
    glBindVertexArray(vao);

    GLint textureLocation = glGetUniformLocation(program, "sampler");
    glUniform1i(textureLocation, 0);
    GLint widthLocation = glGetUniformLocation(program, "width");
    glUniform1i(widthLocation, width);
    GLint heightLocation = glGetUniformLocation(program, "height");
    glUniform1i(heightLocation, height);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);
    glDrawArrays(GL_TRIANGLES, 0, numVertices);
    glBindTexture(GL_TEXTURE_2D, 0);
    glBindVertexArray(0);
    glUseProgram(0);
}
