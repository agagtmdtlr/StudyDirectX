#pragma once
#include <algorithm>
#include <math.h>
#include <glm/glm.hpp>

// 수학의 4차원 벡터(vector)
struct Vertex
{
	glm::vec4 pos;
	glm::vec2 uv;
};

template<int length, typename T> struct vec_template;

template<typename T>
struct vec_template<2,T>
{
	union
	{
		struct{T x,y; };
		struct{T r,g; };
	};

	static int length() { return 2;} 

	constexpr T& operator[](const uint8_t id)
	{		
		switch (id)
		{
		case 0:
		return x;
		case 1:
		return y;
		}
	}
};


struct Vec4
{
	union
	{
		struct{ float x,y,z,w; };
		struct{ float r,g,b,a; };
		float v[4];
	};
	
	float& operator[](const uint8_t id)
	{
	}

	Vec4& operator+=(const Vec4& other)
	{
		this->v[0] += other.v[0];
		this->v[1] += other.v[1];
		this->v[2] += other.v[2];
		this->v[3] += other.v[3];

		return *this;
	}

	Vec4& operator/=(const float& mod)
	{
		this->v[0] /= mod;
		this->v[1] /= mod;
		this->v[2] /= mod;
		this->v[3] /= mod;

		return *this;
	}

	Vec4 Multiply(const float& mul)
	{
		Vec4 result = *this;

		result.v[0] *= mul;
		result.v[1] *= mul;
		result.v[2] *= mul;
		result.v[3] *= mul;

		return result;
	}

	Vec4 Clamp(const float minV, const float maxV) const
	{
		Vec4 result(*this);
		result.v[0] = (std::clamp)(result.v[0], minV, maxV );
		result.v[1] = (std::clamp)(result.v[1], minV, maxV );
		result.v[2] = (std::clamp)(result.v[2], minV, maxV );
		result.v[3] = (std::clamp)(result.v[3], minV, maxV );

		return result;
	}

	float Dot(const Vec4& other)
	{
		float result = 0;
		result += this->v[0] * other.v[0];
		result += this->v[1] * other.v[1];
		result += this->v[2] * other.v[2];		

		return result;
	}

	Vec4 Add(const Vec4& other)
	{
		Vec4 result = *this;

		result.v[0] += other.v[0];
		result.v[1] += other.v[1];
		result.v[2] += other.v[2];
		result.v[3] += other.v[3];
		
		return result;
	}
};

struct Vec3
{

};

struct Vec2
{
	float v[2];

	Vec2& operator+=(const Vec2& other)
	{
		this->v[0] += other.v[0];
		this->v[1] += other.v[1];

		return *this;
	}

};


