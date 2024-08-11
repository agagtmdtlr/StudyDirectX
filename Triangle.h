#pragma once

#include "Object.h"

namespace slab
{
	using namespace glm;

	class Triangle : public Object
	{
	public:
		vec3 v0, v1, v2;

	public:
		Triangle()
			: v0(vec3(0.0f)), v1(vec3(0.0f)), v2(vec3(0.0f))
		{
		}

		Triangle(vec3 v0, vec3 v1, vec3 v2)
			:v0(v0), v1(v1), v2(v2)
		{

		}

		virtual Hit CheckRayCollision(Ray& ray) override
		{
			Hit hit = Hit{-1.0f, vec3(0.0), vec3(0.0)};

			vec3 point, faceNormal;
			float t,u,v;

		}

		// 수학 프로그래밍을 좋아하시는 분들은 직접 구현해보시면 좋고,
		// 대부분은 개념만 이해해두시고 활용하는 방향으로 접근하셔도 충분합니다.
		// 잘 이해가 가지 않는다면 여러 자료로 교차 검증하면서 공부하시는 방법도
		// 좋습니다. 참고:
		// https://www.scratchapixel.com/lessons/3d-basic-rendering/ray-tracing-rendering-a-triangle/ray-triangle-intersection-geometric-solution
		bool IntersectRayTriangle(
			const vec3& orig, const vec3& dir,
			const vec3& v0, const vec3& v1,
			const vec3& v2, vec3& point, vec3& faceNormal,
			float& t, float& u, float& v)		
		{

		}
	};
}