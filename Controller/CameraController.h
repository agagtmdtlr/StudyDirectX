#pragma once
#include "Controller.h"
class CameraController : public Controller
{
public:
	virtual ~CameraController() override {}

	virtual void Render() override;

private:
	float accelaration = 1.0f;
};

