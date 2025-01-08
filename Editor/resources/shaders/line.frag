#version 330 core

// In 
in vec3 Normal;
in vec3 FragPos;

// Out
out vec4 FragColor;

uniform vec3 objectColor;

void main()
{
    FragColor = vec4(objectColor, 1.0);
}