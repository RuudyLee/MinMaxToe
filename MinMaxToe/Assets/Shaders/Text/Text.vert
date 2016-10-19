#version 420

layout (location = 0) in vec4 in_coord;

out vec2 texcoord;

void main() {
	gl_Position = vec4(in_coord.xy, 0, 1);
	texcoord = in_coord.zw;
}