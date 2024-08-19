#pragma once

#include <glm/glm.hpp>
#include "Object.h"

namespace slab
{
	class Object;

	class Hit
	{
	public:
		float d;			// 과선의 시작부터 충돌 지점 까지의 거리
		glm::vec3 point;	// 충돌한 위치
		glm::vec3 normal;	// 충돌한 위치에서 표면의 수직 벡터
		//glm::vec2 w;		// 삼각형의 barycentric coordinates 저장 변수 (임시)

		glm::vec2 uv; //텍스처 좌표

		std::shared_ptr<slab::Object> obj; // 나중에 물체의 재질 등을 가져오기 위한 포인터

	};
}