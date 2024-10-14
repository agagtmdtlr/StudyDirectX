#pragma once
#include "ConstantBuffer.h"

struct Light
{
	Vector3 position;
	float radius;
	Vector4 color;
	UINT id;
};

class LightManager
{
	static LightManager* Get();
	UINT idGenerator;

	LightManager();
	UINT AddLight(const Light& light);
	void UpdateLight(UINT id, Vector3 position);
	void RemoveLight(UINT id);

	void Update();

	std::array<Light,1024> lights;
	ConstantBuffer buffer;
	std::vector<UINT> unusedIDs;

	bool dirty = false;
};

