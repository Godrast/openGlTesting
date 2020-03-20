#version 330 core
// worls pos
struct Material {
    sampler2D diffuse;
    sampler2D specular;
    sampler2D emission;
    float shininess;
}; 



struct Light {
    vec3 direction;
  
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

uniform Light light;  
  
uniform Material material;
out vec4 FragColor;

in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoords;

  
//uniform vec3 objectColor;
uniform vec3 viewPos;

uniform float time;



void main()
{

    
    //vec3 tempLightPos = vec3(vec4(lightPos , 0.0) * view);


    //diffuse
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(light.direction);//normalize(light.position - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);


    //specular
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect (-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir),0.0), material.shininess);





    vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoords));
    
    vec4 specTexture = texture(material.specular, TexCoords);
    vec3 specular =  vec3 (specTexture)* spec * light.specular;

    vec3 emission = vec3(texture(material.emission, TexCoords + vec2(0, time/2))) * vec3(sin(time)/6 + 0.3);
    
    
    

    vec3 result = (ambient + diffuse + specular + emission);
    FragColor = vec4(result, 1.0);
}





//
//view pos
//in vec3 Normal;
//in vec3 FragPos;
//in vec3 lightPos;
//
//  
//uniform vec3 objectColor;
//uniform vec3 viewPos;
//
//
//
//void main()
//{
//
//    
//    //vec3 tempLightPos = vec3(vec4(lightPos , 0.0) * view);
//
//    vec3 ambient = material.ambient * light.ambient;
//
//
//    vec3 norm = normalize(Normal);
//    vec3 lightDir = normalize(lightPos - FragPos);
//    float diff = max(dot(norm, lightDir), 0.0);
//    vec3 diffuse = (diff * material.diffuse) * light.diffuse;
//
//
//
//    vec3 viewDir = normalize(-FragPos);
//    vec3 reflectDir = reflect (-lightDir, norm);
//    float spec = pow(max(dot(viewDir, reflectDir),0.0), material.shininess);
//    vec3 specular = (material.specular * spec) * light.specular;
//
//    vec3 result = (ambient + diffuse + specular) * objectColor;
//    FragColor = vec4(result, 1.0);
//}