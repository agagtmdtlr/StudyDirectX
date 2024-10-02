#pragma once
#include "Material.h"

class UIMaterial
{
public:
	UIMaterial(Material* material);

	void Render();
private:
	Material* material;
};

