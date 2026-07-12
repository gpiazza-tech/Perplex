#version 330 core

layout (location = 0) out vec4 o_Color;

uniform float u_PixelsPerUnit;

in float v_Slope;
in vec4 v_Color;
in float v_Emission;

in vec2 v_WorldCoord;

vec2 get_pixel_coord()
{
	float x = float(int(v_WorldCoord.x * u_PixelsPerUnit)) / u_PixelsPerUnit;
    float y = float(int(v_WorldCoord.y * u_PixelsPerUnit)) / u_PixelsPerUnit;
	return vec2(x, y);
}

void main()
{
	vec2 pixelCoord = get_pixel_coord();
	float absSlope = abs(v_Slope);

	o_Color = v_Color + v_Emission;

	if (absSlope <= 1.0f)
	{
		float lineY = float(int(pixelCoord.x * v_Slope * u_PixelsPerUnit)) / u_PixelsPerUnit;
		float yDist = abs(lineY - pixelCoord.y);

		if (yDist > 0.0f)
			discard;
	}

	if (absSlope > 1.0f)
	{
		float lineX = float(int(pixelCoord.y / v_Slope * u_PixelsPerUnit)) / u_PixelsPerUnit;
		float xDist = abs(lineX - pixelCoord.x);

		if (xDist > 0.0f)
			discard;
	}
}