#include "stdafx.h"
#include "Light.h"

LightManager* LightManager::Get()
{
	static LightManager* g_lightManager = nullptr;
	if (g_lightManager == nullptr)
	{
		g_lightManager = new LightManager();
	}
	return g_lightManager;
}

LightManager::LightManager()
{
	unusedIDs.resize(1024);
	for(UINT i = 0 ; i < 1024;i++)
		unusedIDs[i] = 1024 - i - 1;

	buffer.Initialize(sizeof(Light) * 1024);
}

UINT LightManager::AddLight(const Light& light)
{
	dirty = true;

	UINT id = UINT_MAX;
	if (unusedIDs.empty() == false)
	{
		id = unusedIDs.back();
		lights[id] = light;
		lights[id].id = id;
		unusedIDs.pop_back();
	}
	return id;
}

void LightManager::UpdateLight(UINT id, Vector3 position)
{
	dirty = true;
	lights[id].position = position;
}

void LightManager::RemoveLight(UINT id)
{
	dirty = true;
	lights[id].id = UINT_MAX;
	unusedIDs.push_back(id);
}

void LightManager::Update()
{
	dirty = false;
	Update_ConstantBuffe_Desc desc;
	desc.data = lights.data();
	desc.dataSize = sizeof(Light) * ( 1024 -  unusedIDs.size() );
	buffer.UpdateData(desc);
}
