#pragma once

struct Transform
{
	Vector3 translation;
	Vector3 scale;
	Vector3 rotation;

	Matrix GetWorldMatrix();
};