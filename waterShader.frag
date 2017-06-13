#version 330 core

// This is a sample fragment shader.
out vec4 color;

uniform mat4 model;

in vec3 fragNormal;
in vec3 FragPos;
in vec3 view_direction;

uniform vec3 cameraPos;
uniform samplerCube skybox;


void main()
{
    vec3 norm = normalize(fragNormal);
    vec3 viewDir = (view_direction);
    
    vec3 I = normalize(FragPos - cameraPos);
    vec3 R = reflect(I, normalize(fragNormal));
    
    //air = 1.000277
    //water = 1.38
    
    float air = 1.000277;
    float water = 1.33;
    
//    float R0 = pow((1.000277 - 1.33)/(1.000277 + 1.33), 2);
    //float theta = acos(dot(-viewDir, norm));
    
   // float refract = R0 + (1.0 - R0) * pow(1 - dot(-viewDir, norm), 5);
    
 //   vec3 refraction = I - refract*R;
    vec3 refraction = refract(viewDir, norm, air/water);
//    color = texture(skybox, refraction);
    
    vec3 temp = vec3(mix(texture(skybox, refraction), texture(skybox, R), 0.5));
 
    color = vec4(temp, 0.48);
}


