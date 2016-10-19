#version 420

uniform mat4 CameraPosition;

// Light
uniform vec4 LightPosition;
uniform vec3 LightAmbient;
uniform vec3 LightDiffuse;
uniform vec3 LightSpecular;
uniform float LightAttenuationConstant;
uniform float LightAttenuationLinear;
uniform float LightAttenuationQuadratic;
uniform float LightSpecularExponent;

uniform sampler2D uTex;

in vec2 texcoord;
in vec3 norm;
in vec3 pos;

out vec4 outColor;

void main() {
	// account for rasterizer interpolating
	vec3 normal = normalize(norm);

	outColor.rgb = LightAmbient;

	// Compute lighting
	vec3 lightVec = (CameraPosition * LightPosition).xyz - pos;
	float dist = length(lightVec);
	vec3 lightDir = lightVec / dist;
	float NdotL = dot(normal, lightDir);
	if (NdotL > 0.0) {
		float attenuation = 1.0 / (LightAttenuationConstant 
								+ (LightAttenuationLinear * dist) 
								+ (LightAttenuationQuadratic * dist * dist));

		// Calculate specular contribution
		outColor.rgb += LightDiffuse.rgb * NdotL * attenuation;

		// Blinn-Phong half vector
		float NdotHV = max(dot(normal, normalize(lightDir + normalize(-pos))), 0.0);

		// Calculate diffuse contribution
		outColor.rgb += LightSpecular.rgb * pow(NdotHV, LightSpecularExponent) * attenuation;		
	}

	vec4 textureColor = texture(uTex, texcoord);

	outColor.rgb *= textureColor.rgb;
	outColor.a = textureColor.a;
	
}