#version 330 core

layout(location = 0) in vec3 objectPosition;
layout(location = 1) in vec3 objectNormal;
layout(location = 2) in vec2 objectUV;
layout(location = 3) in float objectType;

out vec3 position;
out vec3 normal;
out vec2 uv;
out float blockType;
out vec4 FragPosLightSpace;

uniform mat4 matrix;
uniform mat4 matrixInverse;

uniform mat4 m_view;
uniform mat4 m_proj;
uniform mat4 lightSpaceMatrix;


void main() {
    vec4 p = matrix * vec4(objectPosition, 1);
    position = vec3(p);
    normal = normalize(transpose(mat3(matrixInverse)) * objectNormal);

    gl_Position = m_proj * m_view * vec4(position, 1);

    uv = objectUV;
    blockType = objectType;

    FragPosLightSpace = lightSpaceMatrix * vec4(position, 1.0);
}
