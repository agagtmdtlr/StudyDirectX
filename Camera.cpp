#include "stdafx.h"
#include "Camera.h"

Matrix Camera::GetProjection()
{
    fov = 3.141592654f / 4.0f;
    SimpleMath::Matrix::CreatePerspectiveFieldOfView(fov,aspectRatio, nearPlane, farPlane);

    return Matrix( XMMatrixPerspectiveFovLH(fov,aspectRatio, nearPlane, farPlane) );
    //return SimpleMath::Matrix::CreateOrthographic(D3D::GetWidth(), D3D::GetHeight(), nearPlane, farPlane);
}

Matrix Camera::GetView()
{
    /*
    Vector3 up = Vector3::Up;
    lookat = Vector3(0,0, 1);
    position = Vector3(0,0,-5);
    Matrix rotationMatrix = Matrix::CreateFromYawPitchRoll(0,0,0);
    lookat = Vector3::Transform(lookat, rotationMatrix);
    up = Vector3::Transform(up, rotationMatrix);
    Matrix view = SimpleMath::Matrix::CreateLookAt(position, target, up);

    Vector3 target = position;
    target += lookat;
    return view;
    */

    XMFLOAT3 up, pos, lookat;
    XMVECTOR upv, posv, lookatv;
    up = XMFLOAT3(0,1,0);
    upv = XMLoadFloat3(&up);
    lookat = XMFLOAT3(0,0,1);
    lookatv = XMLoadFloat3(&lookat);
    pos = XMFLOAT3(0,0,-5);
    posv = XMLoadFloat3(&pos);
    lookatv = XMVectorAdd(posv,lookatv);
    XMMATRIX view = XMMatrixLookAtLH(posv, lookatv, upv);

    Matrix view_m(view);
    return view_m;
}

Matrix Camera::GetViewProjection()
{
    return Matrix();
}
