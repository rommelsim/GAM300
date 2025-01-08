#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexture;
layout (location = 2) in vec3 aNormal;

// layout (location = 2) in flat int textureslot;

out vec2 Texture_Pos;
out vec3 Normal;
out vec3 FragPos;

uniform mat4 model;
uniform mat4 vp;

void main()
{
	gl_Position = vp * model * vec4(aPos, 1.0f);

	Texture_Pos = aTexture;
	Normal = aNormal;
	FragPos = vec3(gl_Position);
}