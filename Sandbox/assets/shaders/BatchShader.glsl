//Flat Color Shader

#type vertex
#version 410 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec4 a_Color;
layout(location = 2) in vec2 a_TexCoord;
layout(location = 3) in float TexIndex;

uniform mat4 u_ViewProjection;
uniform mat4 u_Transform;

out vec4 v_Color;
out vec2 v_TexCoord;
out float v_TexIndex;

void main(){
	v_Color = a_Color;
	v_TexCoord = a_TexCoord;
	v_TexIndex = TexIndex;
	gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);
}


#type fragment
#version 410 core

layout(location = 0) out vec4 a_Color;

in vec4 v_Color;
in vec2 v_TexCoord;
in float v_TexIndex;

uniform sampler2D u_Textures[32];

void main(){
	int index = int(v_TexIndex); 
	a_Color = texture(u_Textures[index], v_TexCoord) * v_Color; 
}