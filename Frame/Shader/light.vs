#version 430 core

layout (location = 0) in vec3 Position;
layout (location = 1) in vec2 textureUV;
layout (location = 2) in vec3 vertex_Normal;

//UV coordinate should be passed to fragment shader
out vec2 UV;
out vec3 Color_onNormal;
out vec3 Normal0;

uniform mat4 gWorld; 
uniform mat4 gWVP;

void main()
{
	//Output the position of Vertex after clipping
    gl_Position = gWVP * vec4(Position, 1.0);

	Normal0 = (gWorld * vec4(vertex_Normal, 0.0)).xyz;
	
	UV = textureUV;
}
