#version 330 core

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


struct Material{

    sampler2D diffuse1;
    sampler2D specular1;
    sampler2D emission;
};

uniform Light lights[MAX_NUM_OF_LIGHTS];  
uniform int numOfLights;
uniform vec3 viewPos;

uniform float time;
uniform Material material;

uniform bool displayEmission = false;


in vec2 TexCoords;
in vec3 Normal;
in vec3 FragPos;

out vec4 FragColor;
out float gl_FragDepth ;

vec3 calculateAmbient(int i, sampler2D diffuse);
vec3 calculateDiffuse(vec3 norm, vec3 lightDir, int i, sampler2D diffuse);
vec3 calculateSpecular(vec3 norm, vec3 lightDir, int i, sampler2D specular, float shininess);



vec2 rotateUV(vec2 uv, float rotation);


void main()
{    


     // FragColor = texture(material.diffuse1, TexCoords);


vec3 emission;

float xRay = 1;

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
        vec3 ambient = calculateAmbient(i, material.diffuse1);
        vec3 diffuse = calculateDiffuse(norm, lightDir, i, material.diffuse1);
        vec3 specular = calculateSpecular(norm,lightDir, i, material.specular1, 64);


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

//                if(dist < 0.5){

//                    xRay = 1-intensity;
//                }
//
                   if(displayEmission){
                        //vec2 uv = rotateUV(TexCoords + vec2(0, time/10), time);

                        emission = vec3(texture(material.emission, TexCoords + vec2(0, time/10) ));// * vec3(sin(time)/4 + 0.25);

                        emission *= attenuation;

                        if(dist < 2){
                            emission *= intensity;
                        } else {
                            emission *= 0;
                        }
                   } 
            }
        }



        result += (ambient + diffuse + specular + emission);
        //}

    }
    if(numOfLights == 0){

        FragColor = texture(material.diffuse1, TexCoords) + texture(material.specular1, TexCoords);

    }else{
        gl_FragDepth = gl_FragCoord.z;
        if(xRay<1){
            //result = result.rgb;
            gl_FragDepth = 1;//(10 * (1-xRay));
        }
        FragColor = vec4(result, 1.0);
    
    }
}




vec3 calculateAmbient(int i, sampler2D diffuse){


    return lights[i].ambient * vec3(texture(diffuse, TexCoords));
}

vec3 calculateDiffuse(vec3 norm, vec3 lightDir, int i, sampler2D diffuse){

    
    float diff = max(dot(norm, lightDir), 0.0);
    return lights[i].diffuse * diff * vec3(texture(diffuse, TexCoords));;
}

vec3 calculateSpecular(vec3 norm, vec3 lightDir, int i, sampler2D specular, float shininess){

    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect (-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir),0.0), shininess);
    return vec3 (texture(specular, TexCoords))* spec * lights[i].specular;
}


vec2 rotateUV(vec2 uv, float rotation){
    float mid = 0.5;
    return vec2(
        cos(rotation) * (uv.x - mid) + sin(rotation) * (uv.y - mid) + mid,
        cos(rotation) * (uv.y - mid) - sin(rotation) * (uv.x - mid) + mid
    );
}
