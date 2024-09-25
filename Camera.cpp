#include "stdafx.h"
#include "Camera.h"

Matrix Camera::GetProjection()
{
    SimpleMath::Matrix::CreatePerspectiveFieldOfView(fov,aspectRatio, nearPlane, farPlane);
    return Matrix();
}

Matrix Camera::GetView()
{
    Vector3 target = position;
    target += lookat;
    Matrix view = SimpleMath::Matrix::CreateLookAt(position, target , Vector3::Up);
    return view;
}

Matrix Camera::GetViewProjection()
{
    return Matrix();
}
