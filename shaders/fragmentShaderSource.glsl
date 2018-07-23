#version 330 core

in vec3 fragColor;
in vec3 normal;
in vec2 uvCoord;

uniform sampler2D tex;

out vec4 color;

void main(){
	color = texture(tex, uvCoord);
}
