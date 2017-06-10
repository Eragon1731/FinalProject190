#version 410 core

//in vec2 TexCoords;

in vec3 FragPos;
in vec3 Normal;
in vec3 view_direction;

uniform vec3 objectColor;  
out vec4 color;

void main(){
   
    //color = vec4 (objectColor, 1.0);
   // color = vec4(result, 1.0);
	color = vec4(0.0f,1.0f,0.0f,1.0f);
}
