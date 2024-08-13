#pragma once

#include <glm/glm.hpp>
#include "Object.h"

namespace slab
{
	using namespace glm;

	class Plane : public Object
	{
	public:
		vec3 normal;
		float dist;

		virtual Hit CheckRayCollision(Ray& ray) override
		{
			Hit hit = Hit{ -1.0f, vec3(0.0), vec3(0.0) };

			vec3 point;
			float t, u, v;
			if (IntersectRayPlane(ray.start, ray.dir, point, t))
			{
				hit.d = dist;
				hit.point = point; // ray.start + ray.dir * t;
				hit.normal = normal;

				// 텍스처링에서 사용
				// hit.uv = uv0 * u + uv1 * v + uv2 * (1.0f - u -v);
			}

			return hit;
		}

		bool IntersectRayPlane(vec3& orig, vec3& dir, vec3& point, float& t)
		{
			// backface culling
			if( dot(-normal, dir) < 0.0f ) return false;

			// 보는 방향과 평행한 평면은 무시한다.
			if( abs(dot(normal, dir)) < 1e-2f) return false;

			t = ( dist - dot(orig,normal) ) / dot(dir, normal);
			point = orig + t * dir;

			return true;
		}

	};
}