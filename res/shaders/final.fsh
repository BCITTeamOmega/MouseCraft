#version 330 core
layout(location = 0) out vec4 result;
in vec2 loc;

uniform sampler2D screenTex;

vec3 tonemap(vec3 inCol) {
    // Simple Reinhard tonemapping
    return inCol / (vec3(1.0f, 1.0f, 1.0f) + inCol);
}

void main()
{
    vec4 samp = texture(screenTex, loc);
    result = vec4(tonemap(samp.rgb), samp.a);
}