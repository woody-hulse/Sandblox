#pragma once

// Defined before including GLEW to suppress deprecation messages on macOS
#ifdef __APPLE__
#define GL_SILENCE_DEPRECATION
#endif
#include <GL/glew.h>
#include <glm/glm.hpp>

#include <unordered_map>
#include <QElapsedTimer>
#include <QOpenGLWidget>
#include <QTime>
#include <QTimer>
#include <GLUT/glut.h>
#include <iostream>

#include "utils/sceneparser.h"
#include "datamanager.h"

#include "settings.h"

#include "cube.h"

#include "terrain.h"

#include "ray.h"

#include "player.h"


class Sandblox : public QOpenGLWidget
{
private:
    GLint m_defaultFBO = 2;                             // CHANGE DEPENDING ON HiDPI

public:
    Sandblox(QWidget *parent = nullptr);
    void finish();                                      // Called on program exit
    void sceneChanged();
    void settingsChanged();
    void saveViewportImage(std::string filePath);
public slots:
    void tick(QTimerEvent* event);                      // Called once per tick of m_timer

protected:
    void initializeGL() override;                       // Called once at the start of the program
    void paintGL() override;                            // Called whenever the OpenGL context changes or by an update() request
    void resizeGL(int width, int height) override;      // Called when window size changes
    void background(glm::vec4 color);
    void crosshairs(glm::vec4 color);
    void drawPrimitives();
    void paintTexture(GLuint texture, bool perPixel, bool kernelBased);
private:
    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void timerEvent(QTimerEvent *event) override;

    // Tick Related Variables
    int m_timer;
    QElapsedTimer m_elapsedTimer;

    // Input Related Variables
    bool m_leftMouseDown = false;
    bool m_rightMouseDown = false;
    glm::vec2 m_prev_mouse_pos;
    std::unordered_map<Qt::Key, bool> m_keyMap;

    Camera camera;
    RenderData renderData;

    // Device Correction Variables
    int m_devicePixelRatio;

    GLuint m_shader;

    GLuint m_fullscreen_vbo;
    GLuint m_fullscreen_vao;

    GLuint m_crosshairs_vbo;
    GLuint m_crosshairs_vao;

    int m_fbo_width;
    int m_fbo_height;

    // Reference Shapes
    Cube cube;
    SceneMaterial basicMaterial;
    SceneGlobalData basicGlobalData;

    static const int sizeX = 5;
    static const int sizeY = 5;
    static const int sizeZ = 5;

    glm::vec4 lightDirection = glm::vec4(-0.5f, -0.8f, 1.f, 0.f);

    void generateTerrain(int terrain[sizeX][sizeY][sizeZ]);
    int terrain_[sizeX][sizeY][sizeZ]; // fix later

    Terrain terrain;
    RayCast rayCast;

    Player player;

    int timerId;

    bool seeMouse = false;

    void verifyVAO(std::vector<GLfloat> &triangleData, GLuint index, GLsizei size, GLsizei stride, const void* offset) {

        int newStride = int(stride / 4);
        int groupNum = 0;
        int newOffset = static_cast<int>(reinterpret_cast<intptr_t>(offset)) / 4;

        for (int i = newOffset; i < triangleData.size(); i = i + newStride) {
            std::cout << "Group " << groupNum << " of Values for VAO index " << index << std::endl;
            std::cout << "[";
            for (auto j = i; j < i + size; ++j) {
                if (j != i + size - 1) {
                    std::cout << triangleData[j]<< ", ";
                } else {
                    std::cout << triangleData[j]<< "]" << std::endl;
                }
            }
            groupNum = groupNum + 1;
        }
        std::cout << "" << std::endl;
    }
};
