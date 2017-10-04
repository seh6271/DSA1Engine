#version 430

layout (location = 0) in vec3 position;
layout (location = 1) in vec2 modelUV;
layout (location = 2) in vec3 normal;

layout (location = 5) uniform mat4 worldView;

layout (location = 8) uniform mat4 transMat;

out vec3 Normal;
out vec2 fragUV;
out vec4 FragPos;


void main()
{
	FragPos = vec4(position, 1);
	gl_Position = worldView * transMat * FragPos;
	Normal = normal;
	fragUV = modelUV;
}