#version 330 core

in vec3 uv;

uniform sampler2D sampler;

uniform int width;
uniform int height;

out vec4 fragColor;

void main()
{
    fragColor = texture(sampler, vec2(uv));
}
