#pragma once

struct vec3
{

	float x, y, z;
	bool operator==(vec3 cmp) const 
	{
		return cmp.x == x && cmp.y == y && cmp.z == z;
	}

};

#define IM_COL32_R_SHIFT    0
#define IM_COL32_G_SHIFT    8
#define IM_COL32_B_SHIFT    16
#define IM_COL32_A_SHIFT    24
#define IM_COL32_A_MASK     0xFF000000

#define IM_COL32(R,G,B,A)    (((unsigned int)(A)<<IM_COL32_A_SHIFT) | ((unsigned int)(B)<<IM_COL32_B_SHIFT) | ((unsigned int)(G)<<IM_COL32_G_SHIFT) | ((unsigned int)(R)<<IM_COL32_R_SHIFT))

struct vec2
{

	float x, y;

};

struct col4
{

	float r, g, b, a;

	unsigned int to_u32()
	{

		return IM_COL32(r * 255.f, g * 255.f, b * 255.f, a * 255.f);

	}

};

__forceinline col4 from_u32(unsigned int col)
{

	col4 ret{};
	ret.r = ((col) & 0xFF) / 255.0;
	ret.g = ((col >> IM_COL32_G_SHIFT) & 0xFF) / 255.0;
	ret.b = ((col >> IM_COL32_B_SHIFT) & 0xFF) / 255.0;
	ret.a = ((col >> IM_COL32_A_SHIFT) & 0xFF) / 255.0;

	return ret;

}

struct boundingbox
{

	bool valid = false;
	float x = 0.f, y = 0.f, w = 0.f, h = 0.f;
	vec2 top_left{};
	vec2 bottom_right{};

};