#version 330 core

layout (location = 0) in vec3 position;

// Uniform variables can be updated by fetching their location and passing values to that location
uniform mat4 projection;
uniform mat4 modelview;

out vec3 TexCoords;

void main()
{
    // OpenGL maintains the D matrix so you only need to multiply by P, V (aka C inverse), and M
    gl_Position = projection * modelview * vec4(position, 1.0);
    //vec4 fragColor = vec4(color, 1.0f);
	//Color = color;
	TexCoords = position; 
}
