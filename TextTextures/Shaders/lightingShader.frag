#version 330 core
// worls pos
struct Material {
    sampler2D diffuse;
    sampler2D specular;
    //sampler2D emission;
    float shininess;
}; 



struct Light {
    vec3 position;
  
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant;
    float linear;
    float quadratic;

    float cutOff;
    float outerCutOff;
    vec3 direction;
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

vec3 calculateAmbient();
vec3 calculateDiffuse(vec3 norm, vec3 lightDir);
vec3 calculateSpecular(vec3 norm, vec3 lightDir);

void main()
{
    

    
    //vec3 tempLightPos = vec3(vec4(lightPos , 0.0) * view);


    //diffuse
    
//    vec3 lightDir;// = normalize(light.direction);//normalize(light.position - FragPos);
////    if(light.position.w == 0.0){
////       lightDir = normalize(vec3(light.position));
////    } else
////    {
//        lightDir = normalize(vec3(light.position) - FragPos);
////    }
//    vec3 norm = normalize(Normal);
    


    //specular







    //vec4 specTexture = texture(material.specular, TexCoords);
    
    //float dist  = length(light.position - FragPos);
    //float attenuation = 1.0 / (light.constant + light.linear * dist + light.quadratic * (dist * dist));



    //vec3 emission = vec3(texture(material.emission, TexCoords + vec2(0, time/2))) * vec3(sin(time)/6 + 0.3);
    
//    ambient  *= attenuation; 
//    diffuse  *= attenuation;
//    specular *= attenuation;

    //only ambient light
//    vec3 result = light.ambient * vec3(texture(material.diffuse, TexCoords));

    vec3 lightDir = normalize(vec3(light.position) - FragPos);

    float theta = dot(lightDir, normalize(-light.direction));
    float epsilon   = light.cutOff - light.outerCutOff;
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0); 
    //float intensity = smoothstep(0.0, 1.0, (theta - light.outerCutOff) / epsilon);
    //if(theta > light.cutOff){
    vec3 norm = normalize(Normal);
    vec3 ambient = calculateAmbient();
    vec3 diffuse = calculateDiffuse(norm, lightDir);
    vec3 specular = calculateSpecular(norm,lightDir);

    float dist  = length(light.position - FragPos);
    float attenuation = 1.0 / (light.constant + light.linear * dist + light.quadratic * (dist * dist));

    ambient  *= attenuation * intensity; 
    diffuse  *= attenuation * intensity;
    specular *= attenuation * intensity;

    vec3 result = (ambient + diffuse + specular);// + emission);
    //}

    FragColor = vec4(result, 1.0);
}

vec3 calculateAmbient(){


    return light.ambient * vec3(texture(material.diffuse, TexCoords));
}

vec3 calculateDiffuse(vec3 norm, vec3 lightDir){

    
    float diff = max(dot(norm, lightDir), 0.0);
    return light.diffuse * diff * vec3(texture(material.diffuse, TexCoords));;
}

vec3 calculateSpecular(vec3 norm, vec3 lightDir){

    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect (-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir),0.0), material.shininess);
    return vec3 (texture(material.specular, TexCoords))* spec * light.specular;
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