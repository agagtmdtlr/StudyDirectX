#pragma once

#include <glm/glm.hpp>

#include "Hit.h"
#include "Ray.h"
#include "Texture.h"

enum class ObjectType
{
	Default = 0,
	Triangle = 1,
	Square = 2
};

namespace slab
{
	using namespace glm;
	class Object
	{
	public:
		//Material
		vec3 amb = vec3(0.f);
		vec3 diff = vec3(0.f);
		vec3 spec = vec3(0.f);
		float alpha = 10.0f;

		std::shared_ptr<Texture> ambTexture;
		std::shared_ptr<Texture> difTexture;

		virtual ObjectType GetObjectType() const { return ObjectType::Default; }

		Object(const vec3& color = {1.0f,1.0f,1.0f})
			:amb(color),diff(color),spec(color)
		{			
		}

		virtual Hit CheckRayCollision(Ray& ray) = 0;
	};
}