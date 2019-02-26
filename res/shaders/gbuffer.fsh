#version 330 core
out vec4 result;
uniform vec3 color;

in vec3 fragNormal;
in vec2 fragTexCoord;

uniform sampler2D texture0;

const vec3 sunDirection = vec3(0.5773502691896258, -0.7071067811865475, -0.4082482904638631);
const vec4 shadowedColor = vec4(0.2f, 0.3f, 0.45f, 1.0f);

void main()
{
    result = mix(shadowedColor, vec4(1.0f, 1.0f, 1.0f, 1.0f), max(0.0f, -dot(fragNormal, sunDirection))) * vec4(color, 1.0f) * texture(texture0, fragTexCoord);
}