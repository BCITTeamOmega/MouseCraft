#version 330 core
layout(location = 0) out vec4 result;

in vec2 loc;

uniform int textureID;
uniform sampler2DArray tex;
uniform vec3 color;

void main()
{
    result = vec4(color, 1.0f) * texture(tex, vec3(loc, textureID));
}