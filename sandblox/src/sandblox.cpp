#include "sandblox.h"

#include <QCoreApplication>
#include <QMouseEvent>
#include <QKeyEvent>
#include <glm/gtx/transform.hpp>
#include <iostream>
#include <random>

#include "utils/shaderloader.h"

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
}

void Sandblox::finish() {
    killTimer(m_timer);
    this->makeCurrent();

    glDeleteProgram(m_shader);

    this->doneCurrent();
}

void Sandblox::makeFBO(){
    glGenTextures(1, &m_fbo_texture);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_fbo_texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_fbo_width, m_fbo_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);

    glGenRenderbuffers(1, &m_fbo_renderbuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, m_fbo_renderbuffer);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, m_fbo_width, m_fbo_height);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);

    glGenFramebuffers(1, &m_fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_fbo_texture, 0);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_fbo_renderbuffer);

    glBindFramebuffer(GL_FRAMEBUFFER, m_defaultFBO);
}

GLuint createTexture(const glm::vec4& color) {
    GLuint textureID;
    glGenTextures(1, &textureID);

    glBindTexture(GL_TEXTURE_2D, textureID);

    unsigned char pixelData[4] = {
        static_cast<unsigned char>(color.r * 255),
        static_cast<unsigned char>(color.g * 255),
        static_cast<unsigned char>(color.b * 255),
        static_cast<unsigned char>(color.a * 255)
    };

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1, 1, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixelData);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glBindTexture(GL_TEXTURE_2D, 0);

    return textureID;
}

void Sandblox::paintUI(UIElement e){
    glUseProgram(m_texture_shader);
    glBindVertexArray(e.vao);

    GLint textureLocation = glGetUniformLocation(m_texture_shader, "sampler");
    glUniform1i(textureLocation, 0);
    GLint widthLocation = glGetUniformLocation(m_texture_shader, "width");
    glUniform1i(widthLocation, size().width() * m_devicePixelRatio);
    GLint heightLocation = glGetUniformLocation(m_texture_shader, "height");
    glUniform1i(heightLocation, size().height() * m_devicePixelRatio);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, e.texture);
    glDrawArrays(GL_TRIANGLES, 0, e.numVertices);
    glBindTexture(GL_TEXTURE_2D, 0);
    glBindVertexArray(0);
    glUseProgram(0);
}

void Sandblox::initUI(GLuint& vao, GLuint& vbo, std::vector<GLfloat> data, int& numVertices) {
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

void Sandblox::background() {

    std::vector<GLfloat> fullscreen_quad_data =
    {
        -1.f,  1.f, 0.f, 0.f, 1.f,
        -1.f, -1.f, 0.f, 0.f, 0.f,
         1.f, -1.f, 0.f, 1.f, 0.f,
         1.f,  1.f, 0.f, 1.f, 1.f,
        -1.f,  1.f, 0.f, 0.f, 1.f,
         1.f, -1.f, 0.f, 1.f, 0.f
    };

    float s1 = 0.0015f;
    float s2 = 0.01f;
    float ar = (float)m_screen_width / m_screen_height;
    std::vector<GLfloat> crosshairs_quad_data =
    {
        -s1,  s2*ar, 0.f, 0.f, 0.f,
        -s1, -s2*ar, 0.f, 0.f, 0.f,
         s1, -s2*ar, 0.f, 0.f, 0.f,
         s1,  s2*ar, 0.f, 0.f, 0.f,
        -s1,  s2*ar, 0.f, 0.f, 0.f,
         s1, -s2*ar, 0.f, 0.f, 0.f,

        -s2,  s1*ar, 0.f, 0.f, 0.f,
        -s2, -s1*ar, 0.f, 0.f, 0.f,
         s2, -s1*ar, 0.f, 0.f, 0.f,
         s2,  s1*ar, 0.f, 0.f, 0.f,
        -s2,  s1*ar, 0.f, 0.f, 0.f,
         s2, -s1*ar, 0.f, 0.f, 0.f
    };

    GLuint m_crosshair_texture = createTexture(glm::vec4(0.6f, 0.6f, 0.6f, 1.f));
    UIOverlay crosshairs(
        std::vector<float>({
         -s1,  s2*ar,
         -s1, -s2*ar,
          s1, -s2*ar,
          s1,  s2*ar,
         -s1,  s2*ar,
          s1, -s2*ar,

         -s2,  s1*ar,
         -s2, -s1*ar,
          s2, -s1*ar,
          s2,  s1*ar,
         -s2,  s1*ar,
          s2, -s1*ar
        }),
        m_crosshair_texture
        );
    UIOverlays.push_back(crosshairs);
    // UIOverlays.push_back(six_b);

    makeFBO();

    initUI(screen_fbo.vao, screen_fbo.vbo, fullscreen_quad_data, screen_fbo.numVertices);
    screen_fbo.texture = m_fbo_texture;
}

void createImageTexture(GLuint& texture, QImage image, int index) {
    glGenTextures(1, &texture);
    glActiveTexture(GL_TEXTURE0 + index);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image.width(), image.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, image.bits());

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glBindTexture(GL_TEXTURE_2D, 0);
}

void Sandblox::updateInventoryUI() {
    inventoryUI.clear();
    glUseProgram(m_shader);
    for (const auto map : textureMap) {
        UIElement block;
        cube.updateParams(map.first);
        cube.drawShape(m_shader);
        block.vao = cube.vao;
        block.vbo = cube.vbo;
        block.texture = map.first;
        block.numVertices = cube.vertexSize();
        block.shapeData.ctm = glm::translate(glm::vec3(30.f, -17.f, -3.8 * 1.5f + map.first * 1.5f));
        if (player.inventorySelection == map.first)
            block.shapeData.ctm *= glm::scale(glm::vec3(1.1f, 1.0f, 1.1f));
        else
            block.shapeData.ctm *= glm::scale(glm::vec3(0.8f, 0.7f, 0.8f));
        block.shapeData.inverseCtm = glm::inverse(block.shapeData.ctm);
        block.shapeData.primitive.material = basicMaterial;
        inventoryUI.push_back(block);
    }
    glUseProgram(0);
}

void Sandblox::initializeGL() {
    m_devicePixelRatio = this->devicePixelRatio();
    m_screen_width = size().width() * m_devicePixelRatio;
    m_screen_height = size().height() * m_devicePixelRatio;
    m_fbo_width = m_screen_width;
    m_fbo_height = m_screen_height;
    m_defaultFBO = 2;

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
    m_texture_shader = ShaderLoader::createShaderProgram(":/resources/shaders/texture.vert", ":/resources/shaders/texture.frag");

    background();

    cube.updateParams(1);

    basicMaterial.clear();
    basicMaterial.cAmbient = glm::vec4(0.87f, 0.56f, 0.2f, 1.f);
    basicMaterial.cDiffuse = glm::vec4(1.f);
    basicMaterial.cSpecular = glm::vec4(0.f);
    basicMaterial.shininess = 0.f;

    basicGlobalData.ka = 0.7f;
    basicGlobalData.kd = 0.2f;
    basicGlobalData.ks = 0.4f;

    QImage grass = QImage(QString(":/resources/textures/grass.png"));
    grass = grass.convertToFormat(QImage::Format_RGBA8888).mirrored();

    QImage dirt = QImage(QString(":/resources/textures/dirt2.png"));
    dirt = dirt.convertToFormat(QImage::Format_RGBA8888).mirrored();

    QImage rock = QImage(QString(":/resources/textures/rock2.png"));
    rock = rock.convertToFormat(QImage::Format_RGBA8888).mirrored();

    QImage ore = QImage(QString(":/resources/textures/ore2.png"));
    ore = ore.convertToFormat(QImage::Format_RGBA8888).mirrored();

    QImage planks = QImage(QString(":/resources/textures/planks.png"));
    planks = planks.convertToFormat(QImage::Format_RGBA8888).mirrored();

    QImage wood = QImage(QString(":/resources/textures/wood.png"));
    wood = wood.convertToFormat(QImage::Format_RGBA8888).mirrored();

    QImage leaves = QImage(QString(":/resources/textures/leaves.png"));
    leaves = leaves.convertToFormat(QImage::Format_RGBA8888).mirrored();

    QImage brick = QImage(QString(":/resources/textures/brick2.png"));
    brick = brick.convertToFormat(QImage::Format_RGBA8888).mirrored();

    textureMap[0] = createTexture(glm::vec4(1.f));
    createImageTexture(textureMap[0], grass, 0);
    textureMap[1] = createTexture(glm::vec4(1.f));
    createImageTexture(textureMap[1], dirt, 1);
    textureMap[2] = createTexture(glm::vec4(1.f));
    createImageTexture(textureMap[2], rock, 2);
    textureMap[3] = createTexture(glm::vec4(1.f));
    createImageTexture(textureMap[3], ore, 3);
    textureMap[4] = createTexture(glm::vec4(1.f));
    createImageTexture(textureMap[4], planks, 4);
    textureMap[5] = createTexture(glm::vec4(1.f));
    createImageTexture(textureMap[5], wood, 5);
    textureMap[6] = createTexture(glm::vec4(1.f));
    createImageTexture(textureMap[6], leaves, 6);
    textureMap[7] = createTexture(glm::vec4(1.f));
    createImageTexture(textureMap[7], brick, 7);

    textureMap_text[1] = createTexture(glm::vec4(1.f));
    textureMap_text[2] = createTexture(glm::vec4(0.f));

    updateInventoryUI();

    terrain = Terrain();
    terrain.generateTerrain();
    //terrain.generateTerrainMesh();

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


    cameraDataUI.pos = glm::vec4(-5.f, 0.f, 0.f, 1.f);
    cameraDataUI.up = glm::vec4(0.f, 1.f, 0.f, 0.f);
    cameraDataUI.heightAngle = renderData.cameraData.heightAngle;
    cameraDataUI.look = glm::vec4(1.f, 0.f, 0.f, 0.f);
    cameraUI = Camera(&cameraDataUI, size().width(), size().height());

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

    glEnable(GL_DEPTH_TEST);

    glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);
    glViewport(0, 0, m_screen_width, m_screen_height);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glUseProgram(m_shader);

    passShapeData(m_shader, renderData.globalData, terrain4.shapeData);
    passLightData(m_shader, lightDirection1, lightDirection2);
    passCameraData(m_shader, camera);

    passTextures(m_shader, textureMap);
    glBindVertexArray(terrain4.shapeData.shape->vao);
    glDrawArrays(GL_TRIANGLES, 0, terrain4.shapeData.numTriangles);
    glBindVertexArray(0);

    glDisable(GL_DEPTH_TEST);
    for (UIElement block : inventoryUI) {
        passCameraData(m_shader, cameraUI);
        passShapeData(m_shader, renderData.globalData, block.shapeData);
        glBindVertexArray(block.vao);
        glDrawArrays(GL_TRIANGLES, 0, cube.vertexSize() / 9);
        glBindVertexArray(0);
    }
    glEnable(GL_DEPTH_TEST);

    glBindTexture(GL_TEXTURE_2D, 0);

    for (UIOverlay overlay : UIOverlays) {
        overlay.paint(m_texture_shader, size().width(), size().height());
    }

    glUseProgram(0);

    glBindFramebuffer(GL_FRAMEBUFFER, m_defaultFBO);
    glViewport(0, 0, size().width() * m_devicePixelRatio, size().height() * m_devicePixelRatio);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    paintUI(screen_fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
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

    //terrain.drawShape(m_shader);
    terrain4.drawShape(m_shader);

    //terrain.shapeData.numTriangles = terrain.vertexSize() / 9;
    terrain4.shapeData.numTriangles = terrain4.vertexSize() / 9;
    //terrain.shapeData.shape = &terrain;
    terrain4.shapeData.shape = &terrain4;

    glUseProgram(0);
}


void Sandblox::sceneChanged() {
    camera = Camera(&renderData.cameraData, size().width(), size().height());
    rayCast = RayCast(&terrain4, &camera);
    player = Player(&terrain4, &camera);

    drawPrimitives();

    update();
}


void Sandblox::keyPressEvent(QKeyEvent *event) {
    m_keyMap[Qt::Key(event->key())] = true;
    if (event->key() == Qt::Key_Escape) seeMouse = !seeMouse;
    if (seeMouse) setCursor(Qt::ArrowCursor);
    else setCursor(Qt::BlankCursor);
}

void Sandblox::keyReleaseEvent(QKeyEvent *event) {
    if (m_keyMap[Qt::Key_P]) {
        if (player.gameMode == GameMode::CREATIVE) player.gameMode = GameMode::ADVENTURE;
        else{
            player.gameMode = GameMode::CREATIVE;
            player.velocity = glm::vec4(0.0f);
        }
    }

    if (m_keyMap[Qt::Key_Q] || m_keyMap[Qt::Key_E]) {

    }

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
            player.inventory[intersectData.blockType - 1] ++;
            drawPrimitives();
        }
    }

    if (event->buttons().testFlag(Qt::RightButton)) {
        m_rightMouseDown = true;
        m_prev_mouse_pos = glm::vec2(event->position().x(), event->position().y());

        rayCast.computeRay(glm::vec2((float)size().width() / 2.f,
                                     (float)size().height() / 2.f), size().width(), size().height());
        IntersectData intersectData = rayCast.intersectRay();
        intersectData.blockType = player.inventorySelection + 1;
        if (intersectData.intersection) { // && (player.inventory[player.inventorySelection] > 0 || player.gameMode == GameMode::CREATIVE)) {
            terrain4.placeBlock(intersectData);
            if (player.collisionDetect(glm::vec3(0.f))) {
                terrain4.breakBlock(intersectData);
            } else {
                // player.inventory[player.inventorySelection] --;
            }
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
    float delta = 0.01;

    // movement
    glm::vec4 move(0.0f);
    if (m_keyMap[Qt::Key_W]) {
        glm::vec4 direction = renderData.cameraData.look;
        direction.y = 0;
        if (player.gameMode == GameMode::ADVENTURE) move += glm::normalize(direction);
        else player.move(glm::normalize(direction) * player.moveSpeed * delta / 2.f);
    }
    if (m_keyMap[Qt::Key_S]) {
        glm::vec4 direction = -renderData.cameraData.look;
        direction.y = 0;
        if (player.gameMode == GameMode::ADVENTURE) move += glm::normalize(direction);
        else player.move(glm::normalize(direction) * player.moveSpeed * delta / 2.f);
    }
    if (m_keyMap[Qt::Key_A]) {
        glm::vec3 direction = -glm::cross(
            glm::vec3(renderData.cameraData.look),
            glm::vec3(renderData.cameraData.up));
        direction.y = 0;
        if (player.gameMode == GameMode::ADVENTURE) move += glm::vec4(glm::normalize(direction), 0.f);
        else player.move(glm::normalize(direction) * player.moveSpeed * delta / 2.f);
    }
    if (m_keyMap[Qt::Key_D]) {
        glm::vec3 direction = glm::cross(
            glm::vec3(renderData.cameraData.look),
            glm::vec3(renderData.cameraData.up));
        direction.y = 0;
        if (player.gameMode == GameMode::ADVENTURE) move += glm::vec4(glm::normalize(direction), 0.f);
        else player.move(glm::normalize(direction) * player.moveSpeed * delta / 2.f);
    }

    if (player.gameMode == GameMode::CREATIVE) player.grounded = false;

    if (m_keyMap[Qt::Key_Space]) {
        if (player.gameMode == GameMode::CREATIVE)
            player.move(glm::vec3(0.0f, 1.0f, 0.0f) * player.moveSpeed * delta / 2.f);
        else
            move += glm::vec4(0.0f, 1.0f, 0.0f, 0.0f);
    }
    if (m_keyMap[Qt::Key_Shift] && player.gameMode == GameMode::CREATIVE)
        player.move(glm::vec3(0.0f, -1.0f, 0.0f) * player.moveSpeed * delta / 2.f);

    if (move != glm::vec4(0.0f)) {
        player.velocity.x = move.x * delta;
        player.velocity.z = move.z * delta;
        if (player.grounded || player.gameMode == GameMode::CREATIVE) {
            player.velocity.y = move.y * delta * 2.f;
            player.grounded = false;
        }
        camera.computeViewMatrix();
    }


    // cross section rotation
    if (m_keyMap[Qt::Key_Q]) {
        float theta = -10.f * delta;
        if (player.gameMode == GameMode::ADVENTURE) {
            glm::vec4 pos = player.camera->data->pos;
            terrain4.crossSection.origin = glm::vec4(
                fmin(fmax(pos.x, 0.f), terrain4.sizeX),
                fmin(fmax(pos.y, 0.f), terrain4.sizeY),
                0.f,
                0.f
                );
        }
        terrain4.rotateCrossSection(theta, 0.f);
        terrain4.generateTerrainMesh();
        player.rectifyPlayer();
        drawPrimitives();
    }

    if (m_keyMap[Qt::Key_E]) {
        float theta = 10.f * delta;
        if (player.gameMode == GameMode::ADVENTURE)
            terrain4.crossSection.origin = player.camera->data->pos;
        terrain4.rotateCrossSection(theta, 0.f);
        terrain4.generateTerrainMesh();
        player.rectifyPlayer();
        drawPrimitives();
    }

    // redraw environment
    if (m_keyMap[Qt::Key_O]) {
        terrain4.generateTerrain4();
        terrain4.generateTerrain();

        terrain4.generateTerrainMesh();
        drawPrimitives();
        camera.data->pos = glm::vec4(terrain4.sizeX / 2.f, terrain4.sizeZ + 2.f, terrain4.sizeY / 2.f, 1.f);
    }

    // inventory
    if (m_keyMap[Qt::Key_1]){
        player.inventorySelection = 0;
        updateInventoryUI();
    }
    if (m_keyMap[Qt::Key_2]) {
        player.inventorySelection = 1;
        updateInventoryUI();
    }
    if (m_keyMap[Qt::Key_3]) {
        player.inventorySelection = 2;
        updateInventoryUI();
    }
    if (m_keyMap[Qt::Key_4]) {
        player.inventorySelection = 3;
        updateInventoryUI();
    }
    if (m_keyMap[Qt::Key_5]) {
        player.inventorySelection = 4;
        updateInventoryUI();
    }
    if (m_keyMap[Qt::Key_6]) {
        player.inventorySelection = 5;
        updateInventoryUI();
    }
    if (m_keyMap[Qt::Key_7]) {
        player.inventorySelection = 6;
        updateInventoryUI();
    }
    if (m_keyMap[Qt::Key_8]) {
        player.inventorySelection = 7;
        updateInventoryUI();
    }

    update();
}
