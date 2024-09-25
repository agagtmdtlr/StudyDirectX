#pragma once

#include <glm/glm.hpp>


namespace slab
{
	class Ray
	{
	public:
		glm::vec3 start; // start position of the ray
		glm::vec3 dir; // direction of the ray
	};
}