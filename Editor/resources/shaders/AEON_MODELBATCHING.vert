#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;
layout (location = 7) in float aEntityID;

layout (std140) uniform Matrices
{
    mat4 vp;
}block_matrices;

out vec2 TexCoords;
out vec3 Normal;
out vec3 FragPos;
flat out int EntityID;

void main()
{
    EntityID = int(aEntityID);
    Normal = aNormal;
    TexCoords = aTexCoords;    
    FragPos = aPos;
    gl_Position = block_matrices.vp * vec4(aPos, 1.0);
}