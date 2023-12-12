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
#include "terrain4.h"

#include "ui.h"
#include "number.h"

#include "ray.h"

#include "player.h"

class Sandblox : public QOpenGLWidget
{
private:
    GLint m_defaultFBO;                             // CHANGE DEPENDING ON HiDPI

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
    void initUI(GLuint& vao, GLuint& vbo, std::vector<GLfloat> data, int& numVertices);
    void background();
    void drawPrimitives();
    void paintUI(UIElement e);
    void updateInventoryUI();
    void makeFBO();
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
    float totalTimeElapsed;

    // Input Related Variables
    bool m_leftMouseDown = false;
    bool m_rightMouseDown = false;
    glm::vec2 m_prev_mouse_pos;
    std::unordered_map<Qt::Key, bool> m_keyMap;

    SceneCameraData cameraDataUI;
    Camera cameraUI;
    Camera camera;
    RenderData renderData;

    // Device Correction Variables
    int m_devicePixelRatio;

    int m_ui_triangles;

    GLuint m_shader;
    GLuint m_texture_shader;
    GLuint m_shadow_map;

    UIElement screen_fbo;
    UIElement crosshair;
    std::vector<UIOverlay> UIOverlays;

    int m_fbo_width;
    int m_fbo_height;

    GLuint m_fbo_texture;
    GLuint m_fbo_renderbuffer;
    GLuint m_fbo;

    unsigned int depthMapFBO;
    unsigned int depthMap;

    GLuint m_screen_width;
    GLuint m_screen_height;

    // Reference Shapes
    Cube cube;
    SceneMaterial basicMaterial;
    SceneGlobalData basicGlobalData;

    glm::vec4 lightDirection1 = glm::vec4(-0.5f, -0.8f, 1.f, 0.f);
    glm::vec4 lightDirection2 = glm::vec4(0.7f, 0.2f, 0.5f, 0.f);
    glm::vec4 lightDirection3 = glm::vec4(1.f, 0.f, 0.f, 0.f);

    glm::vec4 backgroundColor;
    glm::vec4 newBackgroundColor;

    bool survival;

    Terrain terrain;
    Terrain4 terrain4;
    RayCast rayCast;
    std::map<int, GLuint> textureMap;
    std::map<int, GLuint> textureMap_text;
    std::vector<UIElement> inventoryUI;

    Player player;

    int timerId;

    bool seeMouse = false;
};
