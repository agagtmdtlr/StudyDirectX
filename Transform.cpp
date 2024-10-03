#include "stdafx.h"
#include "Transform.h"

Transform::Transform()
{
	translation = Vector3(0, 0, 0);
	rotation = Vector3(0, 0, 0);
	scale = Vector3(1, 1, 1);
}

Matrix Transform::GetWorldMatrix()
{
	// Scale * Rotation * Translation
	Matrix w = Matrix::CreateScale(scale);
	Vector3 r = { XMConvertToRadians(rotation.x), XMConvertToRadians(rotation.y), XMConvertToRadians(rotation.z) };
	w *= Matrix::CreateFromYawPitchRoll(r);
	w *= Matrix::CreateTranslation(translation);
	return w;
}
