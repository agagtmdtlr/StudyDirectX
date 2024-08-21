#pragma once

#include "Object.h"

namespace slab
{
	class Sphere : public Object
	{
	public:
		glm::vec3 center;
		float radius;


		glm::vec3 color; // 뒤에서 "재질(material)"로 확장

		Sphere(const glm::vec3& center, const float radius, const glm::vec3& color = glm::vec3(1.0f))
			: center(center), radius(radius), Object(color)
		{
		}

		// Wikipedia Line–sphere intersection
		// https://en.wikipedia.org/wiki/Line-sphere_intersection
		Hit CheckRayCollision(Ray& ray) override
		{
			Hit hit = Hit{ -1.0f, glm::vec3(0.0f), glm::vec3(0.0f) }; // d가 음수이면 충돌 안한 것으로 가정

			const float b = glm::dot(ray.dir, ray.start - center);
			const float c = glm::dot(ray.start - center, ray.start - center) - radius * radius;

			const float nebla = b * b - c;
			if (nebla >= 0.0f)
			{
				const float d1 = -b + sqrt(nebla);
				const float d2 = -b - sqrt(nebla);
				hit.d = glm::min(d1, d2);
				if( hit.d < 0.0f)
				hit.d = glm::max(d1,d2);

				hit.point = ray.start + ray.dir * hit.d;
				hit.normal = glm::normalize(hit.point - center);
			}

			/*
			* hit.d = .. // 광선의 시작점으로 부터 충돌 지점까지의 거리 (float)
			* hit.point = ... / 광선과 구가 충돌한 지점의 위치 (vec3)
			* hit.normal = .. / 충돌 지점에서 구의 단위 법선 벡터(unit normal vector)
			*/
			return hit;
		}


	};

}



