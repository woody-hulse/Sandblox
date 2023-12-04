#version 330 core

layout(location = 0) in vec3 objectPosition;
layout(location = 1) in vec3 objectNormal;

out vec3 position;
out vec3 normal;

uniform mat4 matrix;
uniform mat4 matrixInverse;

uniform mat4 m_view;
uniform mat4 m_proj;

void main() {
    vec4 p = matrix * vec4(objectPosition, 1);
    position = vec3(p);
    normal = normalize(transpose(mat3(matrixInverse)) * objectNormal);

    gl_Position = m_proj * m_view * vec4(position, 1);
}
