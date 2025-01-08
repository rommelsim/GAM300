#version 400 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;
layout (location = 7) in float aEntityID;

layout (std140) uniform Matrices
{
    mat4 vp;
    mat4 model;
}block_matrices;

out vec2 TexCoords;
out vec3 Normal;
out vec3 FragPos;
//out vec4 FragPosLightSpace;   // position of the mesh in light space (which light?)
flat out int EntityID;

//uniform mat4 lightSpaceMatrix;

void main()
{
    EntityID = int(aEntityID);
    Normal = aNormal;
    TexCoords = aTexCoords;    
    FragPos = vec3(block_matrices.model * vec4(aPos, 1.0));
    //FragPosLightSpace = lightSpaceMatrix * vec4(FragPos, 1.0);
    gl_Position = block_matrices.vp * block_matrices.model * vec4(aPos, 1.0);
}