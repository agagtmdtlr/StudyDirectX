#pragma once
#include "Material.h"
#include "UI.h"

class UIMaterial : public UI
{
public:
	UIMaterial();
	virtual ~UIMaterial() {}
	virtual void Render() override;
private:
	Material* material = nullptr;
};

