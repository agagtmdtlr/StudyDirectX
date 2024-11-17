#include "stdafx.h"
#include "Mesh.h"
#include <memory>
#include <iostream>
#include <algorithm>
#include "PrimitiveBuffer.h"
#include "Renderer.h"

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

void Mesh::RenderMesh()
{
}

void Mesh::Initialize(Transform transform)
{
	this->transform = transform;
}


void Mesh::SetWorldMatrix(Matrix& matrix)
{
	transform = Transform(matrix);
}

PrimitiveBuffer* Mesh::GetBuffer(UINT index)
{
	vector<unique_ptr<PrimitiveBuffer>>& v = buffer->buffers;
	return v[index].get();
}

UINT Mesh::GetBufferCount()
{
	vector<unique_ptr<PrimitiveBuffer>>& v = buffer->buffers;
	return v.size();
}
