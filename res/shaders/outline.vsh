#version 330 core
layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 texCoord;
uniform vec3 color;
uniform mat4 transform;
uniform float lineWidth;

void main()
{
    vec3 offset = vec4(normalize(normal), 1.0).xyz * lineWidth;
    //vec3 offset = (invTransform * vec4(normalize(vec3(0.0, 0.0, 0.0)), 0.0)).xyz * lineWidth;
    gl_Position = transform * vec4(position + offset, 1.0);
    //gl_Position = transform * vec4(position, 1.0);
}