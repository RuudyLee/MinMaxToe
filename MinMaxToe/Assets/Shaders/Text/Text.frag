#version 420

uniform sampler2D uTex;
uniform vec4 uColor;

in vec2 texcoord;

out vec4 outColor;

void main() {
	outColor = vec4(1, 1, 1, texture(uTex, texcoord).r) * uColor;
}