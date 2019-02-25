#version 330 core
layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 texCoord;
uniform vec3 color;
uniform mat4 transform;

out vec3 fragNormal;
out vec2 fragTexCoord;

void main()
{
    gl_Position = transform * vec4(position, 1.0);
    fragNormal = normal;
    fragTexCoord = texCoord;
}