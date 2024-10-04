#pragma once

struct Transform
{
	Vector3 scale = Vector3::One;
	Vector3 rotation = Vector3::Zero;
	Vector3 translation = Vector3::Zero;

	Matrix GetWorldMatrix();
};