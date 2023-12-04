#pragma once


#include <GL/glew.h>
#include "scenedata.h"
#include "../primitive.h"
#include <vector>
#include <string>

struct RenderShapeData {
    ScenePrimitive primitive;
    glm::mat4 ctm;
    glm::mat4 inverseCtm;

    GLuint vbo;
    GLuint vao;
    int numTriangles;

    Primitive* shape;
};

// Struct which contains all the data needed to render a scene
struct RenderData {
    SceneGlobalData globalData;
    SceneCameraData cameraData;

    std::vector<SceneLightData> lights;
    std::vector<RenderShapeData> shapes;
};

class SceneParser {
public:
    // Parse the scene and store the results in renderData.
    // @param filepath    The path of the scene file to load.
    // @param renderData  On return, this will contain the metadata of the loaded scene.
    // @return            A boolean value indicating whether the parse was successful.
    static bool parse(std::string filepath, RenderData &renderData);
};
