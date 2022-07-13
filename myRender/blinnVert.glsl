#version 430

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 texCoord;
layout (location = 2) in vec3 aNormal;

out vec2 tc;
out vec3 varyingNormal;
out vec3 varyingLightDir;
out vec3 varyingVertPos;
out vec3 varyingHalfVector;


uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform vec3 lightPos; 

void main()
{

	varyingVertPos = (model * vec4(aPos,1.0)).xyz;
	varyingLightDir =  lightPos - varyingVertPos;
	varyingNormal = mat3(transpose(inverse(view * model))) * aNormal;  
	
	varyingHalfVector =
		normalize(normalize(varyingLightDir)
		+ normalize(-varyingVertPos)).xyz;
    
    gl_Position = projection * view * model * vec4(aPos, 1.0);
    tc=texCoord;
}