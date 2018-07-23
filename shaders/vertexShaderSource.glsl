#version 330 core

layout (location = 0) in vec3 pos;
layout (location = 1) in vec3 color;
layout (location = 2) in vec3 Normal;
layout (location = 3) in vec2 uv;

uniform mat4 Projection;
uniform mat4 View;
uniform mat4 Model;

out vec3 fragColor;
out vec3 normal;
out vec2 uvCoord;

void main(){
	gl_Position = Projection * View * Model * vec4(pos, 1.0);
	fragColor = color;
	normal = Normal;
	uvCoord = uv;
}
