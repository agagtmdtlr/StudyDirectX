#pragma once
#include "Material.h"
#include "UI.h"

class UIMaterial : UI
{
public:
	UIMaterial(Material* material);
	virtual ~UIMaterial() {}
	virtual void Render() override;
private:
	Material* material;
};

