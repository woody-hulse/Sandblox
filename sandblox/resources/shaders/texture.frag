#version 330 core

// Task 16: Create a UV coordinate in variable
in vec3 uv;

// Task 8: Add a sampler2D uniform
uniform sampler2D textureMap;

// Task 29: Add a bool on whether or not to filter the texture
uniform bool perPix;
uniform bool ker;
uniform vec2 mousePos;

uniform int width;
uniform int height;

out vec4 fragColor;

void main()
{
    fragColor = vec4(0);
    // Task 17: Set fragColor using the sampler2D at the UV coordinate
    if ((abs(uv[0] * width - mousePos[0]) < 2 && abs(uv[1] * height - (height - mousePos[1])) < 10) || (abs(uv[1] * height - (height - mousePos[1])) < 2 && abs(uv[0] * width - mousePos[0]) < 10)){
        fragColor = vec4(0);
    }
    else{
        fragColor = texture(textureMap, vec2(uv));
    }
}
