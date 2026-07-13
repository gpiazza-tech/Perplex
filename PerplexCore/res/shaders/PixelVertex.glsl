#version 330 core

layout (location = 0) in vec3 a_Position;
layout (location = 1) in vec4 a_Color;
layout (location = 2) in float a_Emission;

uniform mat4 u_ViewProj;
uniform mat4 u_Transform;

out vec4 v_Color;
out float v_Emission;

void main()
{
	v_Color = a_Color;
	v_Emission = a_Emission;
	gl_Position = u_ViewProj * u_Transform * vec4(a_Position, 1.0);
}