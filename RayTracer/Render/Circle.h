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

		// x�� �����̱� ������ ���������� x��ǥ y��ǥ ��� ���� ����
		// screen ��ǥ�迡���� x��ǥ�� y ��ǥ�� int���� float�� ��ȯ
		bool IsInside(const glm::vec2& x)
		{
			// ���� �������� �̿��ؼ� x�� ���ȿ� ����ִ����� �Ǵ�
			float len = glm::length(center - x);
			return len < radius;
			//return glm::distance2(center,x) < radius * radius;
		}
	};
}



