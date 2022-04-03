#version 330 core

struct Material {
   sampler2D diffuse;
   sampler2D specular;
   float shininess;
};

struct DirLight {
   vec3 direction;
  
   vec3 ambient;
   vec3 diffuse;
   vec3 specular;
};

struct PointLight {    
   vec3 position;
   vec3 clq;

   vec3 ambient;
   vec3 diffuse;
   vec3 specular;
};

struct SpotLight {
   vec3  position;
   vec3  direction;
   float cutOff;
   float outerCutOff;
    
   vec3 ambient;
   vec3 diffuse;
   vec3 specular;
	
   vec3 clq;
};

uniform PointLight pointLight;
uniform DirLight dirLight;
uniform SpotLight spotLight;

out vec4 FragColor;

uniform Material material;
uniform int flash_mode;
uniform int point_mode;

uniform vec3 viewPos;

in vec2 TexCoords;
in vec3 Normal;
in vec3 FragPos;


vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir)
{
    vec3 lightDir = normalize(-light.direction);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    // combine results
    vec3 ambient  = light.ambient  * vec3(texture(material.diffuse, TexCoords));
    vec3 diffuse  = light.diffuse  * diff * vec3(texture(material.diffuse, TexCoords));
    vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoords));
    return (ambient + diffuse + specular);
}  

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    // attenuation
    float distance    = length(light.position - fragPos);
    float attenuation = 1.0 / (light.clq.x + light.clq.y * distance + light.clq.z * (distance * distance));    
    // combine results
    vec3 ambient  = light.ambient  * vec3(texture(material.diffuse, TexCoords));
    vec3 diffuse  = light.diffuse  * diff * vec3(texture(material.diffuse, TexCoords));
    vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoords));
    ambient  *= attenuation;
    diffuse  *= attenuation;
    specular *= attenuation;
    return (point_mode == 1 ? (ambient + diffuse + specular) : vec3(0.0f));
}
vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 FragPos, vec3 viewDir)
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
   vec3 reflectDir = reflect(-lightDir, norm);  
   float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
   vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoords));   
        
   float distance = length(light.position - FragPos);
   float attenuation = 1.0 / (light.clq.x + light.clq.y * distance + light.clq.z * (distance * distance));
   
   ambient  *= attenuation; 
   diffuse  *= attenuation * intensity;
   specular *= attenuation * intensity;
    return (flash_mode == 1 ? (ambient + diffuse + specular) : ambient);
}

void main()
{
   vec3 norm = normalize(Normal);
   vec3 viewDir = normalize(viewPos - FragPos);

   // phase 1: Directional lighting
   //  vec3 result = CalcDirLight(dirLight, norm, viewDir);
   // phase 2: Point lights
   vec3 result = CalcPointLight(pointLight, norm, FragPos, viewDir);    
   // phase 3: Spot light
   result += CalcSpotLight(spotLight, norm, FragPos, viewDir);  
    
   FragColor = vec4(result, 1.0);
}