#include "stdafx.h"
#include "Mesh.h"
#include <memory>
#include <iostream>
#include <algorithm>


UINT Mesh::idGenerator = 0;
set<UINT> Mesh::instancesUID;

Mesh::Mesh()
{
	Create();
}

Mesh::Mesh(std::string name, Transform transform)
	:name(name), transform(transform)
{	
	 Create();
	 Initialize(name, transform);
}

Mesh::~Mesh()
{
	Mesh::instancesUID.erase(id);
}

void Mesh::Update()
{	
}

void Mesh::Initialize(string name, Transform transform)
{
	this->transform = transform;
	this->name = name;
}

void Mesh::Create()
{
	id = idGenerator;
	idGenerator++;
	Mesh::instancesUID.insert(id);
}
