#pragma once
#include "Object.h"
#include "Triangle.h"
#include <glm/glm.hpp>

namespace slab
{
	using namespace glm;

	class Square : public Object
	{
	public:
		Triangle triangles[2];
		
		Square(vec3 v0, vec3 v1, vec3 v2, vec3 v3)
		{
			triangles[0].v0 = v0;
			triangles[0].v1 = v1;
			triangles[0].v2 = v2;

			triangles[1].v0 = v0;
			triangles[1].v1 = v2;
			triangles[1].v2 = v3;
		}

		virtual Hit CheckRayCollision(Ray& ray)
		{
			Hit hit = Hit{-1.0f, vec3(0.0f), vec3(0.0f)};

			for (int i = 0; i < 2; i++)
			{
				hit = triangles[i].CheckRayCollision(ray);
				if(hit.d >= 0.0f)
					break;
			}

			return hit;
		}
	};
}