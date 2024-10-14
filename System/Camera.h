#pragma once
class Camera
{
public:
	Matrix GetProjection();
	Matrix GetView();
	Matrix GetViewProjection();
	Ray ScreenPointToRay(Vector2 mousePosition);

	void GetDirectionAndUp(Vector3& dir, Vector3& up);

public:
	Vector3 position = Vector3(0.f);
	Vector3 rotation = Vector3(0.f);

	float fov;
	float aspectRatio;
	float nearPlane = 0.01f;
	float farPlane = 1000.f;


	Matrix matrix;
};

