#pragma once

#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp>
#include <vector>
#include <string>
#include <iostream>


namespace slab
{
	// Bilinear interpolation reference
	// https://www.scratchapixel.com/lessons/mathematics-physics-for-computer-graphics/interpolation/bilinear-filtering

	using namespace glm;

	class Texture
	{
	public:
		int width, height, channels;
		std::vector<uint8_t> image;

		Texture(const std::string& filename);
		Texture(const int& width, const int& height, const std::vector<vec3>& pixels);

		vec3 GetClamped(int i, int j)
		{
			i = glm::clamp(i, 0, width -1);
			j = glm::clamp(j, 0, height - 1);

			const float r = image[(i + width * j) * channels + 0] / 255.0f;
			const float g = image[(i + width * j) * channels + 1] / 255.0f;
			const float b = image[(i + width * j) * channels + 2] / 255.0f;

			return vec3(r,g,b);
		}

		vec3 GetWrapped(int i, int j)
		{
			i %= width;		// i가 width면 0으로 바뀜
			j %= height;	// j가 height면 0으로 바뀜

			if( i < 0)
				i += width; // i가 -1이면 (width -1)로 바뀜
			if( j < 0)
				j += height;

			const float r = image[(i + width * j) * channels + 0] / 255.0f;
			const float g = image[(i + width * j) * channels + 1] / 255.0f;
			const float b = image[(i + width * j) * channels + 2] / 255.0f;

			return vec3(r, g, b);
		}

		vec3 InterplateBilinear(
			const float& dx,
			const float& dy,
			const vec3& c00,
			const vec3& c10,
			const vec3& c01,
			const vec3& c11)
		{
			// ...
			return vec3(1.0f);
		}

		vec3 SamplePoint(const vec2& uv) // Nearest sampling 이라고 부르기도 함
		{
			// range of texture coord uv [0.0, 1.0] x [0.0,1.0]
			// range of image coord xy [-0.5, width -1 + 0.5] x [-0.5, height - 1 + 0.5]
			// integer index range of array 

			vec2 xy = vec2((width-1) * uv.x,(height-1)* uv.y);

			int i = int(xy.x);
			int j = int(xy.y);

			return GetClamped(i,j);
		}
	};
}