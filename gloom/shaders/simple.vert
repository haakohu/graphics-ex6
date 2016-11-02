#version 430 core

in vec3 position;
layout(location = 1) in vec4 vcolor;
out vec4 fragColor;
uniform layout(location = 3) mat4 transform;
void main()
{
    gl_Position = transform * vec4(position, 1.0f);
    fragColor = vcolor;
}
