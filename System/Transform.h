#pragma once

struct Transform
{
	Transform(){};
	Transform(Matrix& mat);
	Vector3 scale = Vector3::One;
	Vector3 rotation = Vector3::Zero;
	Vector3 position = Vector3::Zero;

	Matrix GetWorldMatrix();

};