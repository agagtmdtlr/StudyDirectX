#pragma once
#include <glm/glm.hpp>
#include <directxtk/SimpleMath.h>
class Camera
{
private:
	glm::vec3 position;
	glm::vec3 rotation;

	glm::mat4x4 GetProjection();


};

