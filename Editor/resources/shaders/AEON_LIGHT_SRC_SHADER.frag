#version 330 core
layout (location = 0) out vec4 color1;
layout (location = 1) out int  color2;

in vec2 TexCoords;
flat in int EntityID;

uniform vec3 color;

// Textures
struct Material
{
    float shininess;
    sampler2D diffuseTexID;
    sampler2D specularTexID;
};
uniform Material material;

void main()
{
	color1 = vec4(color, 1.0) * vec4(texture(material.diffuseTexID, TexCoords).rgb, 1.0);		// Light's texture multipled with light's color
	color2 = EntityID;
}