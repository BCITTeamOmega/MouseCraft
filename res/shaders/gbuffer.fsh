#version 330 core
layout(location = 0) out vec4 albedo;
layout(location = 1) out vec4 normal;
layout(location = 2) out vec4 position;
uniform vec3 color;

in vec3 fragNormal;
in vec2 fragTexCoord;
in vec3 fragPos;

uniform sampler2D texture0;

void main()
{
    albedo = vec4(color, 1.0f) * texture(texture0, fragTexCoord);
    normal = vec4(normalize(fragNormal), 1.0f);
    position = vec4(fragPos, 1.0f);
}