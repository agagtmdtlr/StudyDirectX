#pragma once
#include "Controller.h"

class MaterialController : public Controller
{
public:
	MaterialController();
	virtual ~MaterialController() {}
	virtual void Render() override;
private:
	class Material* material = nullptr;
};

