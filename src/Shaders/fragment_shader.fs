#version 330 core

struct Material {
   sampler2D diffuse;
   sampler2D specular;
   float shininess;
};

struct Light {
   vec3  position;
   vec3  direction;
   float cutOff;
   float outerCutOff;
    
   vec3 ambient;
   vec3 diffuse;
   vec3 specular;
	
   float constant;
   float linear;
   float quadratic;
}; 


out vec4 FragColor;  

uniform Light light;  
uniform Material material;
uniform int on; 

uniform vec3 viewPos;

in vec2 TexCoords;
in vec3 Normal;
in vec3 FragPos;

void main()
{     
   // ambient
   vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));
  	
   // diffuse 
   vec3 norm = normalize(Normal);
   vec3 lightDir = normalize(light.position - FragPos);
   float diff = max(dot(norm, lightDir), 0.0);
   vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoords));   

   float theta = dot(lightDir, normalize(-light.direction));
   float epsilon = light.cutOff - light.outerCutOff;
   float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);   

   // specular
   vec3 viewDir = normalize(viewPos - FragPos);
   vec3 reflectDir = reflect(-lightDir, norm);  
   float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
   vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoords));   
        
   float distance = length(light.position - FragPos);
   float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
   
   ambient  *= attenuation; 
   diffuse  *= attenuation * intensity;
   specular *= attenuation * intensity; 


   vec3 result = ambient + diffuse + specular;
   FragColor = (on == 0 ? vec4(ambient, 1.0) : vec4(result, 1.0));
}