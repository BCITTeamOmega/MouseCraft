#version 330 core
layout(location = 0) out vec4 result;

in vec2 loc;

uniform sampler2D albedoTex;
uniform sampler2D normalTex;
uniform sampler2D positionTex;

const vec3 sunDirection = vec3(0.5773502691896258, -0.7071067811865475, -0.4082482904638631);
const vec3 sunColor = vec3(4.5f, 3.0f, 2.5f);
const vec3 ambientColor = vec3(0.12f, 0.23f, 0.35f);

vec4 tonemap(vec4 inCol) {
    return vec4(inCol.rgb / (vec3(1.0f, 1.0f, 1.0f) + inCol.rgb), inCol.a);
}

void main()
{
    vec4 albedo = texture(albedoTex, loc);
    vec3 normal = texture(normalTex, loc).rgb;
    vec3 position = texture(positionTex, loc).rgb;

    result = tonemap(vec4((max(-dot(normal, sunDirection), 0.0f) * sunColor + ambientColor), 1.0f) * albedo);
}