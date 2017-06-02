#version 410 core

//in vec2 TexCoords;

in vec3 FragPos;
in vec3 Normal;
in vec3 view_direction;

out vec4 color;

uniform vec3 direction;
uniform vec3 diffuse;
uniform vec3 ambient;
uniform vec3 specular;


uniform vec3 amb;
uniform vec3 diff;
uniform vec3 spec;
uniform float shininess;

void main(){
    
    vec3 lightDir = normalize(-direction);
    vec3 reflectDir = reflect(-lightDir, normalize(Normal));
    
    vec3 a = ambient * amb;
    vec3 d = diffuse * max(dot(normalize(Normal), lightDir), 0.0) * diff;
    vec3 sp = specular * pow(max(dot(view_direction, reflectDir),0.0), shininess) * spec;
    
    vec3 result = a + d +sp;
  //  color = vec4 (1.0,0.0,0.0,1.0);
    color = vec4(result, 1.0);
}
