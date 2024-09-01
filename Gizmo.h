#pragma once

#include <ImGuizmo.h>
#include <glm/glm.hpp>

class Gizmo
{
	Gizmo()
	{
		glm::mat4x4 view;
		glm::mat4x4 projection;
		glm::mat4x4	matrices;
		int matrixCount;
		ImGuizmo::DrawCubes(&view[0][0], &projection[0][0], &matrices[0][0], 4);
	}
};

