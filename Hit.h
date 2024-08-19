#pragma once

#include <glm/glm.hpp>
#include "Object.h"

namespace slab
{
	class Object;

	class Hit
	{
	public:
		float d;			// ������ ���ۺ��� �浹 ���� ������ �Ÿ�
		glm::vec3 point;	// �浹�� ��ġ
		glm::vec3 normal;	// �浹�� ��ġ���� ǥ���� ���� ����
		//glm::vec2 w;		// �ﰢ���� barycentric coordinates ���� ���� (�ӽ�)

		glm::vec2 uv; //�ؽ�ó ��ǥ

		std::shared_ptr<slab::Object> obj; // ���߿� ��ü�� ���� ���� �������� ���� ������

	};
}