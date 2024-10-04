#include "stdafx.h"
#include "Mesh.h"
#include <memory>
#include <iostream>
#include <algorithm>
#include "PrimitiveBuffer.h"

UINT Mesh::idGenerator = 0;
set<UINT> Mesh::instancesUID;

Mesh::Mesh()
	:material(Material())
{
	id = idGenerator;
	idGenerator++;
	Mesh::instancesUID.insert(id);
}

Mesh::~Mesh()
{
	Mesh::instancesUID.erase(id);
}

void Mesh::Update()
{	
}

void Mesh::Render()
{
	auto dc = D3D::GetDC();

	dc
}

void Mesh::Initialize(string name, Transform transform)
{
	this->transform = transform;
	this->name = name;
}

PrimitiveBuffer* Mesh::GetBuffer()
{
	return buffer.get();
}

UINT Mesh::GetIndexCount()
{
	return buffer->GetIndexCount();
}
