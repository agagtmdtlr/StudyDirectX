#include "stdafx.h"
#include "Transform.h"

Transform::Transform( Matrix& mat)
{
	Quaternion quat;
	mat.Decompose(scale,quat,position);
	rotation = quat.ToEuler();

	rotation.x = XMConvertToDegrees(rotation.x);
	rotation.y = XMConvertToDegrees(rotation.y);
	rotation.z = XMConvertToDegrees(rotation.z);
}

Matrix Transform::GetWorldMatrix()
{
	// Scale * Rotation * Translation
	Matrix w = Matrix::CreateScale(scale);
	Vector3 r = { XMConvertToRadians(rotation.x), XMConvertToRadians(rotation.y), XMConvertToRadians(rotation.z) };
	w *= Matrix::CreateFromYawPitchRoll(r);
	w *= Matrix::CreateTranslation(position);
	return w;
}
