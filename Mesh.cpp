#include "stdafx.h"
#include "Mesh.h"
#include <memory>
#include <iostream>
#include "RenderContext.h"
#include <algorithm>


UINT Mesh::idGenerator = 0;

Mesh::Mesh()
{
	 id = idGenerator;
	 idGenerator++;
}

Mesh::~Mesh()
{
	instancesUID.erase(id);
}

void Mesh::Update()
{
	
}

void Mesh::initialize(string name, Transform transform)
{
	instancesUID.insert(id);
	this->transform = transform;
	this->name = name;
}
