#pragma once

#include <glm/glm.hpp>
#include <vector>
#include "Texture.h"
#include "Object.h"
#include "Square.h"

namespace slab
{
	using namespace glm;
	using namespace std;

	class Cubemap
	{
	public:
		std::vector<Square> planes;
		vec3 center;
		vec3 scale;

		Cubemap(const vec3& center, const vec3& scale );
	};
}