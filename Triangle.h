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
			Hit hit = Hit{ -1.0f, vec3(0.0), vec3(0.0) };

			vec3 point, faceNormal;
			float t, u, v;
			if (IntersectRayTriangle(ray.start, ray.dir, v0, v1, v2, point, faceNormal, t, u, v))
			{
				hit.d = t;
				hit.point = point; // ray.start + ray.dir * t;
				hit.normal = faceNormal;

				// 텍스처링에서 사용
				// hit.uv = uv0 * u + uv1 * v + uv2 * (1.0f - u -v);
			}

			return hit;
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
			/*
			* 기본 전략
			* 삼각형이 놓여있는 평면과 광선의 교점을 찾고,
			* 그 교점이 삼각형 안에 있는지 밖에 있는지를 판단한다.
			*/

			/* 1. 삼각형이 놓여 있는 평면의 수직 벡터 계산 */
			faceNormal = cross(v1 - v0, v2 - v1);
			// 주의 : 삼각형의 넓이가 0일 경우에는 계산할 수 없음
			if (length(faceNormal) / 2.0f < FLT_EPSILON)
				return false;

			faceNormal = normalize(faceNormal);

			// 삼각형의 뒷면을 그리고 싶지 않은 경우 (backface culling)
			if (dot(-faceNormal, dir) < 0.0f)
				return false;



			// 평면과 관선이 수평에 매우 가깝다면 충돌하지 못하는 것으로 판다.
			if (glm::abs(dot(dir, faceNormal)) < 1e-2f) return false;

			/* 2. 광선과 평면의 충돌 위치 계산 */
			// 원점으로 부터의 평면의 최단거리
			float D = -glm::dot(faceNormal, v0);
			t = -(dot(faceNormal, orig) + D) / dot(faceNormal, dir);
			point = orig + t * dir; // 충돌점

			/* 3. 그 충돌 위치가 삼각형 안에 들어 있는 확인 */
			// 방향만 확인하면 되기 때문에 normalize() 생략 가능
			// 아래에서 cross product의 절대값으로 작은 삼각형들의 넓이 계산
			const vec3 normal0 = (cross(v0 - point, v1 - point));
			const vec3 normal1 = (cross(v1 - point, v2 - point));
			const vec3 normal2 = (cross(v2 - point, v0 - point));

			if (dot(normal0, faceNormal) < 0.0f) return false;
			if (dot(normal1, faceNormal) < 0.0f) return false;
			if (dot(normal2, faceNormal) < 0.0f) return false;

			// Barycentric coordinates 계산
			// 텍스처링에서 사용
			// u = ...
			// v = ...

			return true;
		}
	};
}