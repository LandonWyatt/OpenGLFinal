#version 430
layout (location = 0) in vec3 vertPos;
layout (location = 1) in vec3 vertNormal;
layout (location = 2) in vec2 texCoord;
in vec4 varyingColor;
in vec2 tc;
layout(binding = 0) uniform sampler2D samp;
out vec4 fragColor;
in vec3 varyingNormal;
in vec3 varyingVertPos;
struct DirectionalLight {
	vec4 ambient;
	vec4 diffuse;
	vec4 specular;
	vec3 direction;
};
struct Material {
	vec4 ambient;
	vec4 diffuse;
	vec4 specular;
	float shininess;
};
uniform vec4 globalAmbient;
uniform DirectionalLight light;
uniform Material material;
uniform mat4 mv_matrix;
uniform mat4 proj_matrix;
uniform mat4 norm_matrix;

void main(void) {
	vec3 varyingLightDir = -light.direction;
	vec3 varyingHalfVector = (varyingLightDir + (-varyingVertPos)).xyz;

	vec3 L = normalize(light.direction);
	vec3 N = normalize(varyingNormal);
	vec3 V = normalize(-varyingVertPos);
	vec3 H = normalize(varyingHalfVector);

	vec3 R = normalize(reflect(-L,N));

	float cosTheta = dot(L,N);
	float cosPhi = dot(H,N);

	vec3 ambient = ((globalAmbient * material.ambient) + (light.ambient * material.ambient)).xyz;
	vec3 diffuse = light.diffuse.xyz * material.diffuse.xyz * max(cosTheta,0.0);
	vec3 specular = light.specular.xyz * material.specular.xyz * pow(max(cosPhi,0.0), material.shininess * 3.0);

	vec4 varyingColor = vec4((ambient + diffuse + specular), 1.0);

	vec4 textureColor = texture(samp, tc);
	if(textureColor.a < 0.7) {
		discard;
	}
	fragColor = textureColor * varyingColor;
};