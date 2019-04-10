#version 330 core
layout(location = 0) out vec4 result;

in vec2 loc;

uniform sampler2D screenTex;
const float[19] kernel = float[] (0.000265, 0.001013, 0.003312, 0.009244, 0.02203, 0.044833, 0.07791, 0.115615, 0.146508, 0.158542, 0.146508, 0.115615, 0.07791, 0.044833, 0.02203, 0.009244, 0.003312, 0.001013, 0.000265);
const int[19] offsets = int[] (-9, -8, -7, -6, -5, -4, -3, -2, -1, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9);

const float threshhold = 0.44;

vec3 maxComponents(float val, vec3 components) {
    return vec3(max(val, components.r), max(val, components.g), max(val, components.b));
}

vec3 calcBloom() {
    vec2 scale = textureSize(screenTex, 0);
    float offsetAmount = 1.0 / scale.x;
    vec3 bloomAmount = vec3(0.0, 0.0, 0.0);
    for (int i = 0; i < 19; i++) {
        vec3 samp = texture(screenTex, vec2(offsets[i] * offsetAmount + loc.x, loc.y)).rgb;
        samp = maxComponents(threshhold, samp) - vec3(threshhold, threshhold, threshhold);
        bloomAmount += kernel[i] * samp;
    }
    return bloomAmount;
}

void main()
{
    vec4 samp = texture(screenTex, loc);
    result = vec4(calcBloom(), samp.a);
}