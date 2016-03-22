#version 430 core

//--------------input data---------------
in vec2 UV;
in vec3 Normal0;
in vec3 position0;

const int MAX_POINT_LIGHTS = 2;

//--------------output data---------------
out vec4 FragColor;


//-------------type specifier is allowed to be after the inout data-----
struct BaseLight                                                                    
{                                                                                   
    vec3 Color;                                                                     
    float AmbientIntensity;                                                         
    float DiffuseIntensity;                                                         
}; 
 
 struct DirectionalLight                                                             
{                                                                                   
    BaseLight Base;                                                                 
    vec3 Direction;                                                                 
};

struct Attenuation                                                                  
{                                                                                   
    float Constant;                                                                 
    float Linear;                                                                   
    float Exp;                                                                      
};  

struct PointLight                                                                           
{                                                                                           
    BaseLight Base;                                                                         
    vec3 Position;                                                                          
    Attenuation Atten;                                                                      
};

uniform int gNumPointLights;
uniform DirectionalLight gDirectionalLight;
uniform PointLight gPointLights[MAX_POINT_LIGHTS];
uniform sampler2D myTextureSampler;
//the camera pos
uniform vec3 gEyeWorldPos;                                                                  
uniform float gMatSpecularIntensity;                                                        
uniform float gSpecularPower;

vec4 CalcLightInternal(BaseLight Light, vec3 LightDirection, vec3 Normal)
{
	float DiffuseFactor = dot(Normal, -LightDirection);
	vec4 AmbientColor = vec4(Light.AmbientIntensity * vec4(Light.Color, 1.0f));
	
	vec4 DiffuseColor = vec4(0, 0, 0, 0);
	vec4 SpecularColor = vec4(0, 0, 0, 0);
	
	if(DiffuseFactor > 0)
	{
		DiffuseColor = vec4(Light.Color * Light.DiffuseIntensity * DiffuseFactor, 1.0f);
		
		vec3 VertexToEye = normalize(gEyeWorldPos - position0);
		//计算光到顶点的反射角，来和视线到顶点进行对比
		vec3 LightReflect = normalize(reflect(LightDirection, Normal));
		float SpecularFactor = dot(VertexToEye, LightReflect);  
		if (SpecularFactor > 0) {                                                           
            SpecularFactor = pow(SpecularFactor, gSpecularPower);
            SpecularColor = vec4(Light.Color * gMatSpecularIntensity * SpecularFactor, 1.0f);
        }
	}
	
	return (AmbientColor + DiffuseColor + SpecularColor);
}

vec4 CalcDirectionalLight(vec3 Normal)
{
	return CalcLightInternal(gDirectionalLight.Base, gDirectionalLight.Direction, Normal);
}

vec4 CalcPointLight(int Index, vec3 Normal)
{
	vec3 LightDirection = position0 - gPointLights[Index].Position;
	float Distance = length(LightDirection);
	LightDirection = normalize(LightDirection);
	
	vec4 Color = CalcLightInternal(gPointLights[Index].Base, LightDirection, Normal);
	float Attenuation = gPointLights[Index].Atten.Exp * Distance * Distance +
						gPointLights[Index].Atten.Linear * Distance +
						gPointLights[Index].Atten.Constant;
	
	return Color / Attenuation;
}
	
void main()
{
	vec3 Normal = normalize(Normal0);                                                       
    vec4 TotalLight = CalcDirectionalLight(Normal);                                         
                                                                                            
    for (int i = 0 ; i < gNumPointLights ; i++) {                                           
        TotalLight += CalcPointLight(i, Normal);                                            
    }                                                                                       
                                                                                            
    FragColor = texture2D(myTextureSampler, UV.xy) * TotalLight;  
}
