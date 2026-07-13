#version 330 core

layout (location = 0) out vec4 o_Color;

in vec4 v_Color;
in float v_Emission;

void main()
{
	o_Color = v_Color + v_Emission;
}