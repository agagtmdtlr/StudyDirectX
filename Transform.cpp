#include "stdafx.h"
#include "Transform.h"

Matrix Transform::GetWorldMatrix()
{
	// Scale * Rotation * Translation
	Matrix w = Matrix::CreateScale(scale);
	Vector3 r = { XMConvertToRadians(rotation.x), XMConvertToRadians(rotation.y), XMConvertToRadians(rotation.z) };
	w *= Matrix::CreateFromYawPitchRoll(r);
	w *= Matrix::CreateTranslation(translation);
	return w;
}
