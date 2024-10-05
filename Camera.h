#pragma once
class Camera
{
public:
	Matrix GetProjection();
	Matrix GetView();
	Matrix GetViewProjection();


public:
	Vector3 position;
	Vector3 rotation;

	float fov;
	float aspectRatio;
	float nearPlane = 0.01f;
	float farPlane = 1000.f;


	Matrix matrix;
};

