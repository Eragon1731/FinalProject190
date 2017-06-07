#version 330 core

in vec3 TexCoords;
out vec4 color;

uniform samplerCube cube;

void main()
{
	color = texture (cube, TexCoords);

}