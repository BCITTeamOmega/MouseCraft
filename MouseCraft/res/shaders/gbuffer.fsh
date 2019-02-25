#version 330 core
out vec4 result;
uniform vec3 color;

in vec3 fragNormal;
in vec2 fragTexCoord;

uniform sampler2D texture0;

void main()
{
    result = vec4(color, 1.0f) * texture(texture0, fragTexCoord);
}