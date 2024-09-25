#include "stdafx.h"
#include "Transform.h"

Matrix Transform::GetWorldMatrix()
{
	Matrix s = Matrix::CreateScale(scale);
	Matrix r = Matrix::CreateFromYawPitchRoll(rotation);
	Matrix t = Matrix::CreateTranslation(translation);
	return s * r * t;
}
