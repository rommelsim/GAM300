#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 2) in vec2 aTexCoords;
layout (location = 7) in float aEntityID;

out vec2 TexCoords;
flat out int EntityID;

uniform mat4 model;
uniform mat4 vp;

void main()
{
	EntityID = int(aEntityID);
    TexCoords = aTexCoords;    
    gl_Position = vp * model * vec4(aPos, 1.0);
}