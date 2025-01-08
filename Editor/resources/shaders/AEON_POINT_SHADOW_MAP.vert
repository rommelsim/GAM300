#version 330 core
layout (location = 0) in vec3 aPos;

uniform mat4 mdl;

void main()
{
    gl_Position = mdl * vec4(aPos, 1.0);
}