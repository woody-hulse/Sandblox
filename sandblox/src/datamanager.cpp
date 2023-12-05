#include "datamanager.h"

void passCameraData(GLuint& program, Camera& camera) {
    glm::mat4 viewMatrix = camera.getViewMatrix();
    glm::mat4 perspectiveMatrix = camera.getPerspectiveMatrix();

    glm::vec3 m_cameraPosition = camera.data->pos;
    GLint cameraPositionLocation = glGetUniformLocation(program, "cameraPosition");
    glUniform3fv(cameraPositionLocation, 1, &m_cameraPosition[0]);

    GLint viewLocation = glGetUniformLocation(program, "m_view");
    glUniformMatrix4fv(viewLocation, 1, GL_FALSE, &viewMatrix[0][0]);

    GLint projLocation = glGetUniformLocation(program, "m_proj");
    glUniformMatrix4fv(projLocation, 1, GL_FALSE, &perspectiveMatrix[0][0]);
}

void passLightData(GLuint& program, glm::vec4 lightDirection1, glm::vec4 lightDirection2) {
    GLint lightDirection1Location = glGetUniformLocation(program, "lightDirection1");
    glUniform3fv(lightDirection1Location, 1, &lightDirection1[0]);

    GLint lightDirection2Location = glGetUniformLocation(program, "lightDirection2");
    glUniform3fv(lightDirection2Location, 1, &lightDirection2[0]);
}

void passShapeData(GLuint& program, SceneGlobalData& globalData, RenderShapeData& shapeData) {
    GLint modelLocation = glGetUniformLocation(program, "matrix");
    glUniformMatrix4fv(modelLocation, 1, GL_FALSE, &shapeData.ctm[0][0]);

    GLint modelInverseLocation = glGetUniformLocation(program, "matrixInverse");
    glUniformMatrix4fv(modelInverseLocation, 1, GL_FALSE, &shapeData.inverseCtm[0][0]);

    glm::vec4 ka = shapeData.primitive.material.cAmbient * globalData.ka;
    GLint k_aLocation = glGetUniformLocation(program, "k_a");
    glUniform4fv(k_aLocation, 1, &ka[0]);

    glm::vec4 kd = shapeData.primitive.material.cDiffuse * globalData.kd;
    GLint k_dLocation = glGetUniformLocation(program, "k_d");
    glUniform4fv(k_dLocation, 1, &kd[0]);

    glm::vec4 ks = shapeData.primitive.material.cSpecular * globalData.ks;
    GLint k_sLocation = glGetUniformLocation(program, "k_s");
    glUniform4fv(k_sLocation, 1, &ks[0]);

    GLint shininessLocation = glGetUniformLocation(program, "shininess");
    glUniform1f(shininessLocation, shapeData.primitive.material.shininess);
}
