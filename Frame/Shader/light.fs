#version 430 core

//--------------input data---------------
in vec2 UV;
in vec3 Normal0;

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

uniform DirectionLight m_DirectionLight;
uniform sampler2D myTextureSampler;

void main()
{
    float delta = max(0.0, dot(normalize(Normal0),m_DirectionLight.Direction));
	
	vec4 AmbientColor = vec4(m_DirectionLight.directionLightColor, 1.0) * 
							m_DirectionLight.AmbientIntensity;
  
	vec4 DiffuseColor;
	if(delta > 0){
		DiffuseColor = vec4(m_DirectionLight.directionLightColor, 1.0) * 
							m_DirectionLight.DiffuseIntensity * delta;
	}
	else{
		DiffuseColor = vec4(0, 0, 0, 0);
		
	}
	FragColor = texture2D(myTextureSampler, UV) *  (AmbientColor + DiffuseColor);
    
}