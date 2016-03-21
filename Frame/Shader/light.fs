#version 430 core

//--------------input data---------------
in vec2 UV;
in vec3 Normal0;
in vec3 position0;
//--------------output data---------------
out vec4 FragColor;


//-------------type specifier is allowed to be after the inout data-----
struct DirectionLight
{
    vec3 directionLightColor;
    float DiffuseIntensity;
	float AmbientIntensity;
	vec3 Direction;
	
};

struct PointLight
{
	vec3 PointLightColor;
	vec3 pointLightPostion;
	float DiffuseIntensity;
	float AmbientIntensity;
};

uniform DirectionLight m_DirectionLight;
uniform PointLight m_PointLight;
uniform sampler2D myTextureSampler;

void main()
{
	//directionLightColor 
    float dir_delta = max(0.0, dot(normalize(Normal0),m_DirectionLight.Direction));
	
	vec4 dir_AmbientColor = vec4(m_DirectionLight.directionLightColor, 1.0) * 
							m_DirectionLight.AmbientIntensity;
  
	vec4 dir_DiffuseColor;
	if(dir_delta > 0){
		dir_DiffuseColor = vec4(m_DirectionLight.directionLightColor, 1.0) * 
							m_DirectionLight.DiffuseIntensity * dir_delta;
	}
	else{
		dir_DiffuseColor = vec4(0, 0, 0, 0);
		
	}
	//PointLight 
	vec3 temp_dir = normalize(m_PointLight.pointLightPostion - position0);
	
	float poi_delta = max(0.0, dot(normalize(Normal0),temp_dir));
	
	vec4 poi_AmbientColor = vec4(m_PointLight.PointLightColor, 1.0) * 
							m_PointLight.AmbientIntensity;
	
	vec4 poi_DiffuseColor;
	if(poi_delta > 0)
	{
		poi_DiffuseColor = vec4(m_PointLight.PointLightColor, 1.0) *
							m_PointLight.DiffuseIntensity * poi_delta;
	}
	else{
		poi_DiffuseColor = vec4(0, 0, 0, 0);
	}
	
	//calculate the color
	FragColor = texture2D(myTextureSampler, UV) *  (dir_AmbientColor + dir_DiffuseColor +
													poi_AmbientColor + poi_DiffuseColor);
    
}