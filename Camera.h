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
	float nearPlane, farPlane;


	Matrix matrix;
};

