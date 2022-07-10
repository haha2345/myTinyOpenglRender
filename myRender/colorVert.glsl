#version 430
layout (location = 0) in vec3 aPos;


uniform mat4 mvMatrix;
uniform mat4 proMatrix;

void main()
{
	gl_Position = proMatrix * mvMatrix * vec4(aPos, 1.0);
}