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
		enum class SampleType // for cpu ray tracer
		{
			Clamp = 0,
			Wrap = 1
		};

		int width, height, channels;
		SampleType sampleType = SampleType::Clamp;
		std::vector<uint8_t> image;

		
		Texture(const std::string& filename);
		Texture(const int& width, const int& height, const std::vector<vec3>& pixels);
		virtual ~Texture() {}


		vec3 GetPixel(int i, int  j)
		{
			switch (sampleType)
			{
				case SampleType::Clamp:
					return GetClamped(i,j);
				case SampleType::Wrap:
					return GetWrapped(i,j);
			}
		}

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

		vec3 InterpolateBilinear(
			const float& dx,
			const float& dy,
			const vec3& c00,
			const vec3& c10,
			const vec3& c01,
			const vec3& c11)
		{
			// ...

			vec3 a1 = c00 + (c10 - c00) * dx;
			vec3 a2 = c01 + (c11 - c01) * dx;

			return a1 + (a2 - a1) * dy;
		}

		vec3 SamplePoint(const vec2& uv) // Nearest sampling 이라고 부르기도 함
		{
			// range of texture coord uv [0.0, 1.0] x [0.0,1.0]
			// range of image coord xy [-0.5, width -1 + 0.5] x [-0.5, height - 1 + 0.5]
			// integer index range of array 

			vec2 xy = uv * vec2(float(width), float(height)) - vec2(0.5f);

			int i = (int)glm::round(xy.x);
			int j = (int)glm::round(xy.y);

			return GetPixel(i,j);
		}

		vec3 SampleLineaer(const vec2& uv)
		{
			// range of texture coord uv [0.0, 1.0] x [0.0,1.0]
			// range of image coord xy [-0.5, width -1 + 0.5] x [-0.5, height - 1 + 0.5]

			vec2 xy = uv * vec2(float(width), float(height)) - vec2(0.5f);

			const int i = int(glm::floor(xy.x));
			const int j = int(glm::floor(xy.y));


			const float dx = xy.x - float(i);
			const float dy = xy.y - float(j);

			return InterpolateBilinear(dx,dy, GetPixel(i,j), GetPixel(i+1,j), GetPixel(i,j+1), GetPixel(i+1,j+1));
		}
	};
}