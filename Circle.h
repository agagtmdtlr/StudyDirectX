#pragma once

#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp>
#include <glm/gtx/norm.hpp>

namespace slab
{
	class Circle
	{
	public:
		glm::vec2 center;
		float radius;
		float radiusSquared;
		glm::vec4 color;

		Circle(const glm::vec2& center, const float radius, const glm::vec4& color)
			: center(center), radius(radius), color(color)
		{
		}

		// x는 벡터이기 때문에 내부적으로 x좌표 y좌표 모두 갖고 있음
		// screen 좌표계에서는 x좌표와 y 좌표가 int지만 float로 변환
		bool IsInside(const glm::vec2& x)
		{
			// 원의 방정식을 이용해서 x가 원안예 들어있는지로 판단
			float len = glm::length(center - x);
			return len < radius;
			//return glm::distance2(center,x) < radius * radius;
		}
	};
}



