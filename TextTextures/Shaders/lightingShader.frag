#version 330 core
// worls pos
struct Material {
    sampler2D diffuse;
    sampler2D specular;
    //sampler2D emission;
    float shininess;
}; 

#define DIRECTIONAL 0
#define SPOTLIGHT 1
#define POINT 2

#define MAX_NUM_OF_LIGHTS 50

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

    int type;

};

uniform Light lights[MAX_NUM_OF_LIGHTS];  
uniform int numOfLights;
  
uniform Material material;
out vec4 FragColor;

in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoords;

  
//uniform vec3 objectColor;
uniform vec3 viewPos;

uniform float time;

vec3 calculateAmbient(int i);
vec3 calculateDiffuse(vec3 norm, vec3 lightDir, int i);
vec3 calculateSpecular(vec3 norm, vec3 lightDir, int i);

void main()
{
    

    
   //vec3 emission = vec3(texture(material.emission, TexCoords + vec2(0, time/2))) * vec3(sin(time)/6 + 0.3);
    
    vec3 result;

    for(int i=0; i< numOfLights; i++){
        vec3 lightDir;
        if(lights[i].type == DIRECTIONAL){
            lightDir = normalize(-lights[i].direction);
        }
        else {
            lightDir = normalize(vec3(lights[i].position) - FragPos);
        }

        //float intensity = smoothstep(0.0, 1.0, (theta - light.outerCutOff) / epsilon);
        //if(theta > light.cutOff){
        vec3 norm = normalize(Normal);
        vec3 ambient = calculateAmbient(i);
        vec3 diffuse = calculateDiffuse(norm, lightDir, i);
        vec3 specular = calculateSpecular(norm,lightDir, i);


        if(lights[i].type != DIRECTIONAL){
            
            float dist  = length(lights[i].position - FragPos);
            float attenuation = 1.0 / (lights[i].constant + lights[i].linear * dist + lights[i].quadratic * (dist * dist));
        
            ambient  *= attenuation; 
            diffuse  *= attenuation;
            specular *= attenuation;
            if(lights[i].type == SPOTLIGHT){

                float theta = dot(lightDir, normalize(-lights[i].direction));
                float epsilon   = lights[i].cutOff - lights[i].outerCutOff;
                float intensity = clamp((theta - lights[i].outerCutOff) / epsilon, 0.0, 1.0); 

                ambient  *= intensity; 
                diffuse  *= intensity;
                specular *= intensity;
            }
        }



        result += (ambient + diffuse + specular);// + emission);
        //}

    }

    FragColor = vec4(result, 1.0);

}

vec3 calculateAmbient(int i){


    return lights[i].ambient * vec3(texture(material.diffuse, TexCoords));
}

vec3 calculateDiffuse(vec3 norm, vec3 lightDir,int i){

    
    float diff = max(dot(norm, lightDir), 0.0);
    return lights[i].diffuse * diff * vec3(texture(material.diffuse, TexCoords));;
}

vec3 calculateSpecular(vec3 norm, vec3 lightDir,int i){

    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect (-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir),0.0), material.shininess);
    return vec3 (texture(material.specular, TexCoords))* spec * lights[i].specular;
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