#version 330 core

#define near 0.1f
#define far 100.0f

out vec4 color;

float getLinearDepth(float depth)
{
	float z = (depth * 2.0) - 1.0;	// convert to NDC
	return (2.0 * near * far) / (far + near - (z * (far - near)));
}

void main()
{
	//color = vec4(vec3(gl_FragCoord.z), 1.0f);
	float depth = getLinearDepth(gl_FragCoord.z) / far;
	color = vec4(vec3(depth), 1.0f);
}
