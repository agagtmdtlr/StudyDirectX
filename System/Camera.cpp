#include "stdafx.h"
#include "Camera.h"

Matrix Camera::GetProjection()
{

	return SimpleMath::Matrix::CreatePerspectiveFieldOfView(XMConvertToRadians(fov), aspectRatio, nearPlane, farPlane);

	//return Matrix(XMMatrixPerspectiveFovLH(fov, aspectRatio, nearPlane, farPlane));
	//return SimpleMath::Matrix::CreateOrthographic(D3D::GetWidth(), D3D::GetHeight(), nearPlane, farPlane);
}

Matrix Camera::GetView()
{
	Vector3 target = position;
	Vector3 up = Vector3::Up;
	Vector3 lookat = Vector3(0, 0, 1);
	Matrix rotationMatrix = Matrix::CreateFromYawPitchRoll(rotation);
	lookat = Vector3::Transform(lookat, rotationMatrix);
	up = Vector3::Transform(up, rotationMatrix);
	target += lookat;
	Matrix view = SimpleMath::Matrix::CreateLookAt(position, target, up);

	return view;
}

Matrix Camera::GetViewProjection()
{
	Matrix vp = GetView();
	vp *= GetProjection();
	return vp;
}

Ray Camera::ScreenPointToRay(Vector2 mousePos)
{
	Vector3 relative = Vector3(mousePos.x, mousePos.y, 1) / Vector3(D3D::GetWidth(), D3D::GetHeight(), 1);
	relative.x -= 0.5f;
	relative.y = 1.0f - relative.y;
	relative.y -= 0.5f;

	// angle
	float verticalAngle = 0.5 * XMConvertToRadians(fov);

	float worldHeight = 2.f * tan(verticalAngle);

	Vector3 worldUnits = relative;
	worldUnits *= worldHeight;
	worldUnits.x *= aspectRatio;
	worldUnits.z = 1;

	Matrix rotationMatrix = Matrix::CreateFromYawPitchRoll(rotation);

	Vector3 direction = Vector3::Transform(worldUnits, rotationMatrix);
	direction.Normalize();

	return Ray(position, direction);
}

void Camera::GetDirectionAndUp(Vector3& dir, Vector3& up)
{
	up = Vector3::Up;
	dir = Vector3(0, 0, 1);
	Matrix rotationMatrix = Matrix::CreateFromYawPitchRoll(rotation);
	dir = Vector3::Transform(dir, rotationMatrix);
	up = Vector3::Transform(up, rotationMatrix);
}
