#shader vertex
#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec4 color;
out vec4 v_Color;

uniform mat4 u_Proj;// Model-View-Projection matrix
uniform mat4 u_View;// Model-View-Projection matrix

void main()
{
	gl_Position =u_Proj * u_View * vec4(position, 1.0);
	v_Color = color;
};


#shader fragment
#version 330 core

layout(location = 0) out vec4 color;
in vec4 v_Color;

//uniform vec4 u_Color;
//uniform sampler2D u_Texture;

void main()
{
	
	color = v_Color;
};