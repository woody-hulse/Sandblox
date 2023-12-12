#version 330 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 objectNormal;
layout(location = 2) in vec2 objectUV;
layout(location = 3) in float objectType;

uniform mat4 lightSpaceMatrix;
uniform mat4 matrix;

void main()
{
    gl_Position = lightSpaceMatrix * matrix * vec4(aPos, 1.0);
}
