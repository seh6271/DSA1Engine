#version 430

out vec4 fragColor; 

in vec3 Normal;
in vec2 fragUV;
in vec4 FragPos;

layout (location = 6) uniform vec3 lightPos;
layout (location = 7) uniform vec3 camPos;


const vec3 ambientColor = vec3(0.1, 0.1, 0.1);
const vec3 diffuseColor = vec3(0.5, 0.5, 0.5);
const vec3 specColor = vec3(1.0, 1.0, 1.0);
const float alpha = 16.0;

uniform sampler2D myTexture;

void main()
{    
	vec3 vertexPos = vec3(FragPos);	
	
	
	vec3 L = normalize(lightPos - vertexPos);
	vec3 V = normalize(camPos - vertexPos);
	vec3 H = normalize(L + V);
	vec3 N = normalize(Normal);
	
	float diffuseScalar = max(dot(L,N),0);
	float specularScalar = pow(max(dot(H,N), 0),alpha);
	
	vec4 lighting = vec4(ambientColor + diffuseScalar * diffuseColor + specularScalar * specColor, 1);

	fragColor = texture2D(myTexture, fragUV) * lighting;
}  