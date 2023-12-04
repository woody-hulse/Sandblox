#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>

#include "glm/gtx/string_cast.hpp"

#include "utils/scenedata.h"
#include "utils/sceneparser.h"
#include "camera.h"

void passCameraData(GLuint& program, Camera& camera);

void passLightData(GLuint& program, glm::vec4 lightDirection);

void passShapeData(GLuint& program, SceneGlobalData& globalData, RenderShapeData& shapeData);
