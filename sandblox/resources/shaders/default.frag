#version 330 core

in vec3 position;
in vec3 normal;
in vec2 uv;
in float blockType;
in vec4 FragPosLightSpace;

uniform sampler2D samplers[8];

out vec4 fragColor;

uniform vec4 k_a;
uniform vec4 k_d;
uniform vec4 k_s;

uniform vec3 lightDirection1;
uniform vec3 lightDirection2;

uniform vec3 cameraPosition;
uniform float shininess;

uniform sampler2D shadowMap;

float shadowCalc(){
    vec3 pos = FragPosLightSpace.xyz * 0.5 + 0.5;
    float depth = texture(shadowMap, pos.xy).r;
    if (pos.z > 1.0){
        pos.x = 1.0;
    }
    float bias = 0.05;
    return (depth + bias) < pos.z ? 0.0 : 1.0;
}

float falloff(float x, float theta_inner, float theta_outer) {
    float z = (x - theta_inner) / (theta_outer - theta_inner);
    float falloff = -2 * pow(z, 3) + 3 * pow(z, 2);

    if (x <= theta_inner) return 1;
    else if (x <= theta_outer) return 1 - falloff;
    else return 0;
}

float attenuation(vec3 function, float distance) {
    return min(1, 1 / (function.x + distance * function.y + distance * distance * function.z));
}

void main() {
    float shadow = shadowCalc();

    fragColor = vec4(0.0f, 0.0f, 0.0f, 0.0f);

    vec4 color = texture(samplers[int(blockType) - 1], uv);

    fragColor += 0.5f * color;

    fragColor += 0.5f * color * min(max(dot(normalize(normal), -lightDirection1), 0.0f), 1.0f);

    vec3 viewDirection = normalize(cameraPosition - position);
    vec3 reflectedLight = reflect(lightDirection1, normalize(normal));
    float specularTerm = min(max(dot(viewDirection, reflectedLight), 0.0), 1.0f);
    if (shininess > 0) specularTerm = pow(specularTerm, shininess);
    else specularTerm = 1;
    fragColor +=  k_s * specularTerm;

    fragColor += shadow * 0.5f * color * min(max(dot(normalize(normal), -lightDirection2), 0.0f), 1.0f);

    viewDirection = normalize(cameraPosition - position);
    reflectedLight = reflect(lightDirection2, normalize(normal));
    specularTerm = min(max(dot(viewDirection, reflectedLight), 0.0), 1.0f);
    if (shininess > 0) specularTerm = pow(specularTerm, shininess);
    else specularTerm = 1;
    fragColor += shadow * k_s * specularTerm;
}
