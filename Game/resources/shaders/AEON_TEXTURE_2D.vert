#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aTexCoord;

out vec3 Color;
out vec2 TexCoord;

uniform mat4 vp;
uniform mat4 model;

void main()
{
	gl_Position = vp * model * vec4(aPos, 1.0);
	Color = aColor;
	TexCoord = vec2(aTexCoord.x, aTexCoord.y);
}