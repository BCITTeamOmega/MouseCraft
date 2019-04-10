#version 330 core
layout(location = 0) out vec4 result;

#define MAX_LIGHTS 50

in vec2 loc;

uniform sampler2D albedoTex;
uniform sampler2D positionTex;
uniform sampler2D normalTex;
uniform sampler2D outlineTex;

uniform int numLights;
uniform vec3 ambientColor;

struct Light {
    int type;
    vec4 color;
    vec4 position;
    vec4 direction;
    vec4 attenuation;
};

layout (std140) uniform Lights {
    Light light[MAX_LIGHTS];
};

vec3 tonemap(vec3 inCol) {
    // Simple Reinhard tonemapping
    return inCol / (vec3(1.0f, 1.0f, 1.0f) + inCol);
}

vec3 calcDiffuseRadiance(Light l, vec3 albedo, vec3 position, vec3 normal) {
    vec3 posDiff = position - l.position.xyz;

    float distance = length(posDiff);
    vec3 distances = vec3(1.0f, distance, distance * distance);

    // The stuff below might look strange, but it's just calculating the strength and direction such that
    // It will account for directional and point lights without using any branching
    float strength = float(l.type != 0) * (1.0 / dot(l.attenuation.xyz, distances)) + float(l.type == 0);
    vec3 direction = float(l.type != 0) * normalize(posDiff) + float(l.type == 0) * l.direction.xyz;

    float lum = max(0.0, -dot(normal, direction)) * strength;
    float avgColLum = length(l.color.rgb);
    if (lum * avgColLum > 0.85) {
        return l.color.rgb * albedo;
    }
    else {
        return step(0.25, lum) * 0.45 * l.color.rgb * albedo;
    }
}

vec3 calcSpecularRadiance(Light l, vec3 albedo, vec3 position, vec3 normal) {
    float shininess = 16;
    vec3 posDiff = position - l.position.xyz;

    float distance = length(posDiff);
    vec3 distances = vec3(1.0f, distance, distance * distance);

    // The stuff below might look strange, but it's just calculating the strength and direction such that
    // It will account for directional and point lights without using any branching
    float strength = float(l.type != 0) * (1.0 / dot(l.attenuation.xyz, distances)) + float(l.type == 0);
    vec3 toLight = float(l.type != 0) * normalize(posDiff) + float(l.type == 0) * l.direction.xyz;

    vec3 halfVector = normalize(-toLight + normalize(-l.position.xyz));
    float lum = pow(max(dot(normal, halfVector), 0.0f), shininess) * strength;
    float avgColLum = length(l.color.rgb);
    if (lum * avgColLum > 0.8) {
        return l.color.rgb;
    }
    else {
        return step(0.3, lum) * 0.3 *  l.color.rgb;
    }
}

vec3 calcRadiance(Light l, vec3 albedo, vec3 position, vec3 normal) {
    return calcDiffuseRadiance(l, albedo, position, normal) + calcSpecularRadiance(l, albedo, position, normal);
}

void main()
{
    vec4 albedo = texture(albedoTex, loc);
    vec3 position = texture(positionTex, loc).rgb;
    vec3 normal = texture(normalTex, loc).rgb;
    vec4 outline = texture(outlineTex, loc);

    if (outline.a > 0.02) {
        result = outline;
    }
    else {
        vec3 totalRadiance = vec3(0.0f, 0.0f, 0.0f);
        for (int i = 0; i < numLights; i++) {
            Light l = light[i];
            totalRadiance += calcRadiance(l, albedo.rgb, position, normal);
        }
        totalRadiance += ambientColor * albedo.rgb;

        result = vec4(tonemap(totalRadiance), albedo.a);
    }
}