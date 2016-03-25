#version 430 core

//--------------input data---------------
in vec2 UV;
in vec3 Normal0;
in vec3 position0;
in vec4 LightSpacePos;

const int MAX_POINT_LIGHTS = 2;
const int MAX_SPOT_LIGHTS = 2; 

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

struct SpotLight
{
	PointLight Base;
	vec3 Direction;
	float Cutoff;
};

uniform int gNumPointLights;
uniform int gNumSpotLights;
uniform DirectionalLight gDirectionalLight;
uniform PointLight gPointLights[MAX_POINT_LIGHTS];
uniform SpotLight gSpotLights[MAX_SPOT_LIGHTS];
uniform sampler2D myTextureSampler;
uniform sampler2D gShadowMap;

//the camera pos
uniform vec3 gEyeWorldPos;                                                                  
uniform float gMatSpecularIntensity;                                                        
uniform float gSpecularPower;

float CalcShadowFactor(vec4 LightSpacePos)
{
	vec3 ProjCoords = LightSpacePos.xyz / LightSpacePos.w;                                  
    vec2 UVCoords;                                                                          
    UVCoords.x = 0.5 * ProjCoords.x + 0.5;                                                  
    UVCoords.y = 0.5 * ProjCoords.y + 0.5;                                                  
    float z = 0.5 * ProjCoords.z + 0.5;                                                     
    float Depth = texture(gShadowMap, UVCoords).x;                                          
    if (Depth < z + 0.00001)                                                                 
        return 0.5;                                                                         
    else                                                                                    
        return 1.0;     
}

vec4 CalcLightInternal(BaseLight Light, vec3 LightDirection, vec3 Normal, float ShadowFactor)
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
	
	return (AmbientColor + ShadowFactor * ( DiffuseColor + SpecularColor));
}

vec4 CalcDirectionalLight(vec3 Normal)
{
	return CalcLightInternal(gDirectionalLight.Base, gDirectionalLight.Direction, Normal, 1.0);
}

vec4 CalcPointLight(PointLight l, vec3 Normal, vec4 LightSpacePos)
{
	vec3 LightDirection = position0 - l.Position;
	float Distance = length(LightDirection);
	LightDirection = normalize(LightDirection);
	
	float ShadowFactor = CalcShadowFactor(LightSpacePos);
	
	vec4 Color = CalcLightInternal(l.Base, LightDirection, Normal, ShadowFactor);
	float Attenuation = l.Atten.Exp * Distance * Distance +
						l.Atten.Linear * Distance +
						l.Atten.Constant;
	
	return Color / Attenuation;
}
	
vec4 CalcSpotLight(SpotLight l, vec3 Normal, vec4 LightSpacePos)
{
	vec3 LightToPixel = normalize(position0 - l.Base.Position);
	float SpotFactor = dot(LightToPixel, l.Direction);
	
	if(SpotFactor > l.Cutoff)
	{
		vec4 Color = CalcPointLight(l.Base, Normal, LightSpacePos);
		return Color * (1.0 - (1.0 - SpotFactor) * 1.0/(1.0 - l.Cutoff));                   
    }                                                                                       
    else {                                                                                  
        return vec4(0,0,0,0);                                                               
    }            
}
void main()
{
	vec3 Normal = normalize(Normal0);                                                       
    vec4 TotalLight = CalcDirectionalLight(Normal);                                         
                                                                                            
    for (int i = 0 ; i < gNumPointLights ; i++) {                                           
        TotalLight += CalcPointLight(gPointLights[i], Normal, LightSpacePos);                                            
    }                                                                                       
    for (int i = 0 ; i < gNumSpotLights ; i++) {                                            
        TotalLight += CalcSpotLight(gSpotLights[i], Normal, LightSpacePos);                 
    }             
	
    FragColor = texture2D(myTextureSampler, UV.xy) * TotalLight;  
}
