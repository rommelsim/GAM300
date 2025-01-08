#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec4 aColor;

out vec3 v_Pos;
out vec4 v_Color;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;


void main()
{
	v_Color = aColor;
	gl_Position = projection * view * model * vec4(aPos, 1.0);
}