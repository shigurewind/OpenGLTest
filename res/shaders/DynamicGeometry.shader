#shader vertex
#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec4 color;
layout(location = 2) in vec2 texcoord;
layout(location = 3) in float texIndex;

out vec4 v_Color;
out vec2 v_Texcoord;
out float v_TexIndex;

uniform mat4 u_Proj;// Model-View-Projection matrix
uniform mat4 u_View;// Model-View-Projection matrix

void main()
{
	gl_Position =u_Proj * u_View * vec4(position, 1.0);
	v_Color = color;
	v_Texcoord = texcoord;
	v_TexIndex = texIndex;
};


#shader fragment
#version 330 core

layout(location = 0) out vec4 color;
in vec4 v_Color;
in vec2 v_Texcoord;
in float v_TexIndex;

//uniform vec4 u_Color;
uniform sampler2D u_Textures[2];

void main()
{
	int index = int(v_TexIndex);	

	color = texture(u_Textures[index], v_Texcoord);
};