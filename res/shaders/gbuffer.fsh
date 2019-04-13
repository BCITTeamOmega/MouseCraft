#version 330 core
layout(location = 0) out vec4 albedo;
layout(location = 1) out vec4 normal;
layout(location = 2) out vec4 position;
layout(location = 3) out vec4 specular;

uniform vec3 color;
uniform int textureID;
uniform float shininess;
uniform float smoothness;

in vec3 fragNormal;
in vec2 fragTexCoord;
in vec3 fragPos;

uniform sampler2DArray albedoTex;

void main()
{
    albedo = vec4(color, 1.0f) * texture(albedoTex, vec3(fragTexCoord, textureID));
    normal = vec4(normalize(fragNormal), 1.0f);
    position = vec4(fragPos, 1.0f);
    specular = vec4(shininess, smoothness, 0.0f, 0.0f);
}