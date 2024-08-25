#include "Cubemap.h"

namespace slab
{
	using namespace std;
	using namespace glm;

	Cubemap::Cubemap(const vec3& center, const vec3& scale)
	{
		vec2 uv01 = vec2(0, 1);
		vec2 uv00 = vec2(0, 0);
		vec2 uv10 = vec2(1, 0);
		vec2 uv11 = vec2(1, 1);

		float x = -1;
		float y = -1;
		float z = 1;
		Square front = Square(
			vec3(-1, -1, -1), vec3(-1, 1, -1), vec3(1, 1, -1), vec3(1, -1, -1),
			uv01, uv00, uv10, uv11
		);
		Square back = Square(
			vec3(-1, -1, 1), vec3(-1, 1, 1), vec3(1, 1, 1), vec3(1, -1, 1),
			uv11, uv10, uv00, uv11
		);

		Square left = Square(
			vec3(-1, -1, 1), vec3(-1, 1, 1), vec3(1, 1, 1), vec3(1, -1, 1),
			uv01, uv00, uv10, uv11
		);

		for (int i = 0; i < 6; i++)
		{

			Square square = Square(
				vec3(-1, -1, -1), vec3(-1, 1, -1), vec3(1, 1, -1), vec3(1, -1, -1),
				vec2(0, 1), vec2(0, 0), vec2(1, 0), vec2(1, 1)
			);
		}
	}

}
