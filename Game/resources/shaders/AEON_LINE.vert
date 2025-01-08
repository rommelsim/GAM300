#version 330 core
layout (location = 0) in vec3 aPos;				// "r-value"
layout (location = 1) in vec3 aNormal;
// layout (location = 2) in int index;

uniform mat4 vp;								// has addr, slow to use.
uniform mat4 model;
// ssbo matrices


out vec4 pos;

void main()
{
	gl_Position = vp * model * vec4(aPos, 1.0);
	
	pos = gl_Position;
}