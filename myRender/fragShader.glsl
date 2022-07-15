#version 430

out vec4 FragColor;

in vec2 tc;  
in vec3 varyingNormal;
in vec3 varyingLightDir;
in vec3 varyingVertPos;
  
struct Light{
    vec3 direction;
};

uniform Light light;
uniform vec3 lightPos; 
uniform vec3 viewPos; 
uniform vec3 lightColor;
uniform vec3 objectColor;
layout (binding=0) uniform sampler2D s;

void main()
{

    // normalize the light, normal, and view vectors:
	vec3 L = normalize(-light.direction);
	vec3 N = normalize(varyingNormal);
	vec3 V = normalize(-varyingVertPos);

    vec3 varyingHalfVector =
		normalize(normalize(L)
		+ normalize(-varyingVertPos)).xyz;

    // get the angle between the light and surface normal:
	float cosTheta = dot(L,N);

    // halfway vector varyingHalfVector was computed in the vertex shader,
	// and interpolated prior to reaching the fragment shader.
	// It is copied into variable H here for convenience later.
	vec3 H = normalize(varyingHalfVector);

    // get angle between the normal and the halfway vector
	float cosPhi = dot(H,N);

    // ambient
    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * lightColor;
    //vec3 ambient = ambientStrength * lightColor;
  	
    // diffuse 
    float diff = max(cosTheta, 0.0);
    vec3 diffuse = diff * lightColor;
    
    // specular
    float specularStrength = 0.5;
    
    float spec = pow(max(cosPhi,0.0), 32);

    
    vec3 specular = specularStrength * spec * lightColor;  
        
    vec3 result = (ambient + diffuse + specular) * texture(s,tc).rgb;
    FragColor = vec4(result, 1.0);
} 