#version 330 core

layout (location = 0) out vec4 o_Color;

uniform float u_PixelsPerUnit;

in vec2 v_Center;
in vec2 v_Bounds;
in vec4 v_Color;
in float v_Emission;

in vec2 v_WorldCoord;

float make_pixel_perfect(in float val)
{
	return float(int(val * u_PixelsPerUnit)) / u_PixelsPerUnit;
}

vec2 make_pixel_perfect(in vec2 pos)
{
	float x = float(int(pos.x * u_PixelsPerUnit)) / u_PixelsPerUnit;
    float y = float(int(pos.y * u_PixelsPerUnit)) / u_PixelsPerUnit;

	return vec2(x, y);
}

void main()
{
	vec2 pixelSize = vec2(1.0f / u_PixelsPerUnit, 1.0f / u_PixelsPerUnit);

	// Transform all values to first quadrant
	vec2 offset = v_Bounds - v_Center + pixelSize / 1.0f;

	float top = make_pixel_perfect(offset.y + v_Center.y + v_Bounds.y);
	float bottom = make_pixel_perfect(offset.y + v_Center.y - v_Bounds.y);
	float right = make_pixel_perfect(offset.x + v_Center.x + v_Bounds.x);
	float left = make_pixel_perfect(offset.x + v_Center.x - v_Bounds.x);

	vec2 pixelCoord = make_pixel_perfect(offset + v_WorldCoord);

	o_Color = v_Color + v_Emission + u_PixelsPerUnit / 20000.0f;

	if (top != pixelCoord.y && bottom != pixelCoord.y && left != pixelCoord.x && right != pixelCoord.x)
		discard;
	if (pixelCoord.y > top || pixelCoord.y < bottom || pixelCoord.x < left || pixelCoord.x > right)\
		discard;
}