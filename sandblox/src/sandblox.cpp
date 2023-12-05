#include "sandblox.h"

#include <QCoreApplication>
#include <QMouseEvent>
#include <QKeyEvent>
#include <glm/gtx/transform.hpp>
#include <iostream>

#include "utils/shaderloader.h"


// ================== Project 5: Lights, Camera

Sandblox::Sandblox(QWidget *parent)
    : QOpenGLWidget(parent)
{
    m_prev_mouse_pos = glm::vec2(size().width()/2, size().height()/2);
    setMouseTracking(true);
    setFocusPolicy(Qt::StrongFocus);

    m_keyMap[Qt::Key_W]       = false;
    m_keyMap[Qt::Key_A]       = false;
    m_keyMap[Qt::Key_S]       = false;
    m_keyMap[Qt::Key_D]       = false;
    m_keyMap[Qt::Key_Control] = false;
    m_keyMap[Qt::Key_Space]   = false;

    // If you must use this function, do not edit anything above this
}

void Sandblox::finish() {
    killTimer(m_timer);
    this->makeCurrent();

    glDeleteProgram(m_shader);

    this->doneCurrent();
}

void Sandblox::background(glm::vec4 color) {
    std::vector<GLfloat> fullscreen_quad_data =
        { //     POSITIONS    //
            -1.f,  1.f, 0.0f, 0.f, 1.f,
            -1.f, -1.f, 0.0f, 0.f, 0.f,
            1.f, -1.f, 0.0f, 1.f, 0.f,
            1.f,  1.f, 0.0f, 1.f, 1.f,
            -1.f,  1.f, 0.0f, 0.f, 1.f,
            1.f, -1.f, 0.0f, 1.f, 0.f
        };

    glGenBuffers(1, &m_fullscreen_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, m_fullscreen_vbo);
    glBufferData(GL_ARRAY_BUFFER, fullscreen_quad_data.size()*sizeof(GLfloat), fullscreen_quad_data.data(), GL_STATIC_DRAW);
    glGenVertexArrays(1, &m_fullscreen_vao);
    glBindVertexArray(m_fullscreen_vao);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), nullptr);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), reinterpret_cast<void*>(3 * sizeof(GLfloat)));

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}


void Sandblox::generateTerrain(int terrain[sizeX][sizeY][sizeZ]) {
    for (int i = 0; i < sizeX; i++) {
        for (int j = 0; j < sizeY; j++) {
            for (int k = 0; k < sizeZ; k++) {
                terrain[i][j][k] = 1;
            }
        }
    }
}

void Sandblox::initializeGL() {
    m_devicePixelRatio = this->devicePixelRatio();

    m_timer = startTimer(1000/60);
    m_elapsedTimer.start();

    glewExperimental = GL_TRUE;
    GLenum err = glewInit();
    if (err != GLEW_OK) {
        std::cerr << "Error while initializing GL: " << glewGetErrorString(err) << std::endl;
    }
    std::cout << "Initialized GL: Version " << glewGetString(GLEW_VERSION) << std::endl;

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glViewport(0, 0, size().width() * m_devicePixelRatio, size().height() * m_devicePixelRatio);
    setCursor(Qt::BlankCursor);

    m_shader = ShaderLoader::createShaderProgram(":/resources/shaders/default.vert", ":/resources/shaders/default.frag");

    cube.updateParams(1, 1);

    basicMaterial.clear();
    basicMaterial.cAmbient = glm::vec4(0.87f, 0.56f, 0.2f, 1.f);
    basicMaterial.cDiffuse = glm::vec4(1.f);
    basicMaterial.cSpecular = glm::vec4(0.f);
    basicMaterial.shininess = 0.f;

    basicGlobalData.ka = 0.7f;
    basicGlobalData.kd = 0.2f;
    basicGlobalData.ks = 0.4f;

    generateTerrain(terrain_);

    terrain = Terrain();
    terrain.generateTerrain();
    terrain.generateTerrainMesh();

    terrain4 = Terrain4();
    terrain4.generateTerrain4();
    terrain4.generateTerrain();
    terrain4.generateTerrainMesh();

    ScenePrimitive primitive;
    primitive.type = PrimitiveType::PRIMITIVE_MESH;
    terrain.shapeData.primitive = primitive;
    float scale = 1.f;
    terrain.shapeData.ctm = glm::mat4(1.f);
    terrain.shapeData.inverseCtm = glm::inverse(terrain.shapeData.ctm);
    terrain.shapeData.primitive.material = basicMaterial;

    ScenePrimitive primitive4;
    primitive4.type = PrimitiveType::PRIMITIVE_MESH;
    terrain4.shapeData.primitive = primitive4;
    float scale4 = 1.f;
    terrain4.shapeData.ctm = glm::mat4(1.f);
    terrain4.shapeData.inverseCtm = glm::inverse(terrain4.shapeData.ctm);
    terrain4.shapeData.primitive.material = basicMaterial;

    renderData.globalData = basicGlobalData;
    renderData.cameraData.pos = glm::vec4(terrain.sizeX / 2.f, terrain.sizeZ + 5.f, terrain.sizeY / 2.f, 1.f);
    renderData.cameraData.up = glm::vec4(0.f, 1.f, 0.f, 0.f);
    renderData.cameraData.heightAngle = 1.f / 3.f * 3.1415f;
    renderData.cameraData.look = glm::vec4(-4.f, 0.f, -3.f, 0.f);

    player = Player();

    drawPrimitives();

    sceneChanged();
}

void Sandblox::paintGL() {

    int elapsedms   = m_elapsedTimer.elapsed();
    float deltaTime = elapsedms * 0.001f;
    m_elapsedTimer.restart();
    camera.computeViewMatrix();
    player.simulate(deltaTime);

    glUseProgram(m_shader);

    passShapeData(m_shader, renderData.globalData, terrain4.shapeData);
    passLightData(m_shader, lightDirection1, lightDirection2);
    passCameraData(m_shader, camera);

    glBindVertexArray(terrain4.shapeData.shape->vao);
    glDrawArrays(GL_TRIANGLES, 0, terrain4.shapeData.numTriangles);
    //std::cout << shapeData.numTriangles << std::endl;
    glBindVertexArray(0);

    glUseProgram(0);
}

void Sandblox::resizeGL(int w, int h) {
    glViewport(0, 0, size().width() * m_devicePixelRatio, size().height() * m_devicePixelRatio);

    camera.setAspectRatio(size().width() * m_devicePixelRatio, size().height() * m_devicePixelRatio);

    int m_screen_width = size().width() * m_devicePixelRatio;
    int m_screen_height = size().height() * m_devicePixelRatio;
    m_fbo_width = m_screen_width;
    m_fbo_height = m_screen_height;
}

void Sandblox::drawPrimitives() {
    glClearColor(0.3f, 0.4f, 0.9f, 1.0f);
    glUseProgram(m_shader);

    terrain.drawShape(m_shader);
    terrain4.drawShape(m_shader);

    terrain.shapeData.numTriangles = terrain.vertexSize() / 6;
    terrain4.shapeData.numTriangles = terrain4.vertexSize() / 6;
    terrain.shapeData.shape = &terrain;
    terrain4.shapeData.shape = &terrain4;

    glUseProgram(0);
}


void Sandblox::sceneChanged() {
    camera = Camera(&renderData.cameraData, size().width(), size().height());
    rayCast = RayCast(&terrain4, &camera);
    player = Player(&terrain4, &camera);
    std::cout << glm::to_string(camera.getPerspectiveMatrix()) << std::endl;
    drawPrimitives();

    //timerId = startTimer(1);

    update();
}


void Sandblox::keyPressEvent(QKeyEvent *event) {
    m_keyMap[Qt::Key(event->key())] = true;
    if (event->key() == Qt::Key_Escape) seeMouse = !seeMouse;
    if (seeMouse) setCursor(Qt::ArrowCursor);
    else setCursor(Qt::BlankCursor);
}

void Sandblox::keyReleaseEvent(QKeyEvent *event) {
    m_keyMap[Qt::Key(event->key())] = false;
}

void Sandblox::mousePressEvent(QMouseEvent *event) {
    if (event->buttons().testFlag(Qt::LeftButton)) {
        m_leftMouseDown = true;
        m_prev_mouse_pos = glm::vec2(event->position().x(), event->position().y());

        rayCast.computeRay(glm::vec2((float)size().width() / 2.f,
                                     (float)size().height() / 2.f), size().width(), size().height());
        IntersectData intersectData = rayCast.intersectRay();
        if (intersectData.intersection) {
            terrain4.breakBlock(intersectData);
            drawPrimitives();
        }
    }

    if (event->buttons().testFlag(Qt::RightButton)) {
        m_rightMouseDown = true;
        m_prev_mouse_pos = glm::vec2(event->position().x(), event->position().y());

        rayCast.computeRay(glm::vec2((float)size().width() / 2.f,
                                     (float)size().height() / 2.f), size().width(), size().height());
        IntersectData intersectData = rayCast.intersectRay();
        if (intersectData.intersection) {
            terrain4.placeBlock(intersectData);
            if (player.collisionDetect(glm::vec3(0.f)))
                terrain4.breakBlock(intersectData);
            drawPrimitives();
        }
    }
}

void Sandblox::mouseReleaseEvent(QMouseEvent *event) {
    if (!event->buttons().testFlag(Qt::LeftButton)) {
        m_leftMouseDown = false;
    }

    if (!event->buttons().testFlag(Qt::RightButton)) {
        m_rightMouseDown = false;
    }
}

glm::mat4 rotate(glm::vec3 vector, float theta) {
    float cosTheta = glm::cos(theta);
    float sinTheta = glm::sin(theta);

    float x = vector.x;
    float y = vector.y;
    float z = vector.z;

    glm::mat4 rotationMatrix(1.0f);

    rotationMatrix[0][0] = cosTheta + x * x * (1 - cosTheta);
    rotationMatrix[1][0] = x * y * (1 - cosTheta) - z * sinTheta;
    rotationMatrix[2][0] = x * z * (1 - cosTheta) + y * sinTheta;

    rotationMatrix[0][1] = y * x * (1 - cosTheta) + z * sinTheta;
    rotationMatrix[1][1] = cosTheta + y * y * (1 - cosTheta);
    rotationMatrix[2][1] = y * z * (1 - cosTheta) - x * sinTheta;

    rotationMatrix[0][2] = z * x * (1 - cosTheta) - y * sinTheta;
    rotationMatrix[1][2] = z * y * (1 - cosTheta) + x * sinTheta;
    rotationMatrix[2][2] = cosTheta + z * z * (1 - cosTheta);

    return rotationMatrix;
}

void Sandblox::mouseMoveEvent(QMouseEvent *event) {
    if (!seeMouse) {
        int posX = event->position().x();
        int posY = event->position().y();
        float deltaX = (posX - m_prev_mouse_pos.x) * 0.01f;
        float deltaY = (posY - m_prev_mouse_pos.y) * 0.01f;
        m_prev_mouse_pos = glm::vec2(posX, posY);

        glm::mat4 xRotation = rotate(-glm::vec3(0.0f, 1.0f, 0.0f), deltaX);
        glm::mat4 yRotation = rotate(-glm::normalize(glm::cross(
                                         glm::normalize(glm::vec3(renderData.cameraData.look)),
                                         glm::normalize(glm::vec3(renderData.cameraData.up)))), deltaY);

        renderData.cameraData.look = xRotation * renderData.cameraData.look;
        renderData.cameraData.look = yRotation * renderData.cameraData.look;
        camera.computeViewMatrix();
    }

    update();
}

void Sandblox::timerEvent(QTimerEvent *event) {
    int elapsedms   = m_elapsedTimer.elapsed();
    float deltaTime = elapsedms * 0.001f;

    glm::vec4 move(0.0f);

    if (m_keyMap[Qt::Key_W])
        move += glm::normalize(renderData.cameraData.look);
    if (m_keyMap[Qt::Key_S])
        move += -glm::normalize(renderData.cameraData.look);
    if (m_keyMap[Qt::Key_A])
        move += -glm::vec4(glm::normalize(glm::cross(
                               glm::vec3(renderData.cameraData.look),
                               glm::vec3(renderData.cameraData.up))), 0.f);
    if (m_keyMap[Qt::Key_D])
        move += glm::vec4(glm::normalize(glm::cross(
                              glm::vec3(renderData.cameraData.look),
                              glm::vec3(renderData.cameraData.up))), 0.f);
    move.y = 0;
    if (m_keyMap[Qt::Key_Space])
        move += glm::vec4(0.0f, 0.5f, 0.0f, 0.0f);

    if (move != glm::vec4(0.0f)) {
        player.velocity.x = move.x * deltaTime;
        player.velocity.z = move.z * deltaTime;
        if (player.grounded && move.y > 0) {
            player.velocity.y = move.y * deltaTime * 2.f;
            player.grounded = false;
        }
        camera.computeViewMatrix();

        /* add this
        float epsilon = 0.001;
        float delta = camera.data->pos.x - terrain4.crossSection.origin.x;
        terrain4.crossSection.origin.x += delta;
        terrain4.crossSection.origin.y += delta * terrain4.crossSection.direction.y;
        */
    }

    if (m_keyMap[Qt::Key_Q]) {
        float theta = -1.f * deltaTime;
        terrain4.rotateCrossSection(theta, 0.f);
        std::cout << deltaTime << " " << glm::to_string(terrain4.crossSection.direction) << std::endl;
        terrain4.generateTerrainMesh();
        drawPrimitives();
    }

    if (m_keyMap[Qt::Key_E]) {
        float theta = 1.f * deltaTime;
        terrain4.rotateCrossSection(theta, 0.f);
        terrain4.generateTerrainMesh();
        drawPrimitives();
    }

    update();
}

// DO NOT EDIT
void Sandblox::saveViewportImage(std::string filePath) {
    // Make sure we have the right context and everything has been drawn
    makeCurrent();

    int fixedWidth = 1024;
    int fixedHeight = 768;

    // Create Frame Buffer
    GLuint fbo;
    glGenFramebuffers(1, &fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);

    // Create a color attachment texture
    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, fixedWidth, fixedHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture, 0);

    // Optional: Create a depth buffer if your rendering uses depth testing
    GLuint rbo;
    glGenRenderbuffers(1, &rbo);
    glBindRenderbuffer(GL_RENDERBUFFER, rbo);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, fixedWidth, fixedHeight);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rbo);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        std::cerr << "Error: Framebuffer is not complete!" << std::endl;
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        return;
    }

    // Adjust the default FBO
    GLuint oldFBO = m_defaultFBO;
    m_defaultFBO = fbo;

    // resize the openGL viewport and propagate new default FBO
    resizeGL(fixedWidth, fixedHeight);

    // Clear and render your scene here
    paintGL();

    // Read pixels from framebuffer
    std::vector<unsigned char> pixels(fixedWidth * fixedHeight * 3);
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);
    glReadPixels(0, 0, fixedWidth, fixedHeight, GL_RGB, GL_UNSIGNED_BYTE, pixels.data());

    // Unbind the framebuffer and return the default FBO
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    m_defaultFBO = oldFBO;
    resizeGL(size().width() * m_devicePixelRatio, size().height() * m_devicePixelRatio);

    // Convert to QImage
    QImage image(pixels.data(), fixedWidth, fixedHeight, QImage::Format_RGB888);
    QImage flippedImage = image.mirrored(); // Flip the image vertically

    // Save to file using Qt
    QString qFilePath = QString::fromStdString(filePath);
    if (!flippedImage.save(qFilePath)) {
        std::cerr << "Failed to save image to " << filePath << std::endl;
    }

    // Clean up
    glDeleteTextures(1, &texture);
    glDeleteRenderbuffers(1, &rbo);
    glDeleteFramebuffers(1, &fbo);
}
