#ifndef UI_H
#define UI_H

#include <GL/glew.h>
#include <vector>
#include "utils/sceneparser.h"
#include "utils/scenedata.h"

struct UIElement {
    int numVertices;
    GLuint vao;
    GLuint vbo;
    GLuint texture;
    RenderShapeData shapeData;
    Primitive* primitive;
};

class UIOverlay
{
public:
    UIOverlay();
    UIOverlay(std::vector<GLfloat> data, GLint texture);
    UIOverlay(std::vector<std::vector<GLint>> data, GLint texture, float x, float y, float sizeX, float sizeY);

    void init();
    void translate(float x, float y, float sizeX, float sizeY);
    void paint(GLuint program, int width, int height);

    GLint texture;
    std::vector<GLfloat> data;

    GLuint vao;
    GLuint vbo;

    int numVertices;
};

#endif // UI_H
