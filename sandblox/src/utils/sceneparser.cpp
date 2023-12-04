#include "sceneparser.h"
#include "scenefilereader.h"
#include <glm/gtx/transform.hpp>

#include <chrono>
#include <iostream>

void traverseSceneGraph(SceneNode node, RenderData& renderData, glm::mat4 ctm, glm::vec4 pos) {
    for (SceneTransformation* transformation : node.transformations) {
        switch (transformation->type) {
        case (TransformationType::TRANSFORMATION_MATRIX):
            ctm = ctm * transformation->matrix;
            break;
        case (TransformationType::TRANSFORMATION_ROTATE):
            ctm = ctm * glm::rotate(transformation->angle, transformation->rotate);
            break;
        case (TransformationType::TRANSFORMATION_SCALE):
            ctm = ctm * glm::scale(transformation->scale);
            break;
        case (TransformationType::TRANSFORMATION_TRANSLATE):
            ctm = ctm * glm::translate(transformation->translate);
            break;
        }
    }

    pos = ctm * pos;

    for (ScenePrimitive* primitive : node.primitives) {
        RenderShapeData data;
        data.primitive = *primitive;
        data.ctm = ctm;
        data.inverseCtm = glm::inverse(ctm);
        renderData.shapes.push_back(data);
    }

    for (SceneLight* light : node.lights) {
        SceneLightData data;
        data.id = light->id;
        data.type = light->type;
        data.color = light->color;
        data.function = light->function;
        data.pos = pos;
        data.dir = light->dir;
        data.penumbra = light->penumbra;
        data.angle = light->angle;
        data.width = light->width;
        data.height = light->height;
        renderData.lights.push_back(data);
    }

    for (SceneNode* child : node.children) {
        traverseSceneGraph(*child, renderData, ctm, pos);
    }
}

bool SceneParser::parse(std::string filepath, RenderData &renderData) {
    ScenefileReader fileReader = ScenefileReader(filepath);
    bool success = fileReader.readJSON();
    if (!success) {
        return false;
    }

    renderData.globalData = fileReader.getGlobalData();
    renderData.cameraData = fileReader.getCameraData();

    SceneNode* rootNode = fileReader.getRootNode();
    glm::mat4 identity(
        1, 0, 0, 0,
        0, 1, 0, 0,
        0, 0, 1, 0,
        0, 0, 0, 1
        );
    renderData.shapes.clear();
    renderData.lights.clear();
    traverseSceneGraph(*rootNode, renderData, identity, glm::vec4(0, 0, 0, 1));

    return true;
}
