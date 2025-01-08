#version 330 core
layout (location = 0) in vec3 vertex; 	// position
layout (location = 1) in vec2 uv;		// texture

out vec2 TexCoords;

uniform mat4 vp;
uniform mat4 mdl;

void main()
{
    gl_Position = vp * mdl * vec4(vertex, 1.0);
    TexCoords = uv;
}  