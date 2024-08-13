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

				// �ؽ�ó������ ���
				// hit.uv = uv0 * u + uv1 * v + uv2 * (1.0f - u -v);
			}

			return hit;
		}

		// ���� ���α׷����� �����Ͻô� �е��� ���� �����غ��ø� ����,
		// ��κ��� ���丸 �����صνð� Ȱ���ϴ� �������� �����ϼŵ� ����մϴ�.
		// �� ���ذ� ���� �ʴ´ٸ� ���� �ڷ�� ���� �����ϸ鼭 �����Ͻô� �����
		// �����ϴ�. ����:
		// https://www.scratchapixel.com/lessons/3d-basic-rendering/ray-tracing-rendering-a-triangle/ray-triangle-intersection-geometric-solution
		bool IntersectRayTriangle(
			const vec3& orig, const vec3& dir,
			const vec3& v0, const vec3& v1,
			const vec3& v2, vec3& point, vec3& faceNormal,
			float& t, float& u, float& v)
		{
			/*
			* �⺻ ����
			* �ﰢ���� �����ִ� ���� ������ ������ ã��,
			* �� ������ �ﰢ�� �ȿ� �ִ��� �ۿ� �ִ����� �Ǵ��Ѵ�.
			*/

			/* 1. �ﰢ���� ���� �ִ� ����� ���� ���� ��� */
			faceNormal = cross(v1 - v0, v2 - v1);
			// ���� : �ﰢ���� ���̰� 0�� ��쿡�� ����� �� ����
			if (length(faceNormal) / 2.0f < FLT_EPSILON)
				return false;

			faceNormal = normalize(faceNormal);

			// �ﰢ���� �޸��� �׸��� ���� ���� ��� (backface culling)
			if (dot(-faceNormal, dir) < 0.0f)
				return false;



			// ���� ������ ���� �ſ� �����ٸ� �浹���� ���ϴ� ������ �Ǵ�.
			if (glm::abs(dot(dir, faceNormal)) < 1e-2f) return false;

			/* 2. ������ ����� �浹 ��ġ ��� */
			// �������� ������ ����� �ִܰŸ�
			float D = -glm::dot(faceNormal, v0);
			t = -(dot(faceNormal, orig) + D) / dot(faceNormal, dir);
			point = orig + t * dir; // �浹��

			/* 3. �� �浹 ��ġ�� �ﰢ�� �ȿ� ��� �ִ� Ȯ�� */
			// ���⸸ Ȯ���ϸ� �Ǳ� ������ normalize() ���� ����
			// �Ʒ����� cross product�� ���밪���� ���� �ﰢ������ ���� ���
			const vec3 normal0 = (cross(v0 - point, v1 - point));
			const vec3 normal1 = (cross(v1 - point, v2 - point));
			const vec3 normal2 = (cross(v2 - point, v0 - point));

			if (dot(normal0, faceNormal) < 0.0f) return false;
			if (dot(normal1, faceNormal) < 0.0f) return false;
			if (dot(normal2, faceNormal) < 0.0f) return false;

			// Barycentric coordinates ���
			// �ؽ�ó������ ���
			// u = ...
			// v = ...

			return true;
		}
	};
}