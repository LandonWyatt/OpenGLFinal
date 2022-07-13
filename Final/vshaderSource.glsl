#version 430
layout (location = 0) in vec3 vertPos;
layout (location = 1) in vec3 vertNormal;
layout (location = 2) in vec2 texCoord;
out vec2 tc;
out vec4 varyingColor;
out vec3 varyingNormal;
out vec3 varyingVertPos;
layout(binding = 0) uniform sampler2D samp;
struct Material {
	vec4 ambient;
	vec4 diffuse;
	vec4 specular;
	float shininess;
};
uniform vec4 globalAmbient;
uniform Material material;
uniform mat4 mv_matrix;
uniform mat4 proj_matrix;
uniform mat4 norm_matrix; // for transforming normals

void main(void) { 
	varyingVertPos = (mv_matrix * vec4(vertPos, 1.0)).xyz;
	varyingNormal = (norm_matrix * vec4(vertNormal, 1.0)).xyz;

	// send the position to the fragment shader, as before
	gl_Position = proj_matrix * mv_matrix * vec4(vertPos,1.0);
	tc = texCoord;
}