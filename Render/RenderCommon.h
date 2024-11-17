#pragma once
#include <algorithm>
#include <math.h>
#include <glm/glm.hpp>
#include <vector>
#include <d3d11.h>
#include <d3dcompiler.h>


struct Vertex
{
	Vertex() {}
	Vertex(Vector3 p, Vector3 n, Vector2 uv) : position(p), normal(n), uv(uv) {}
	Vector3 position;
	Vector3 normal;
	Vector2 uv;
};


struct MatrixBufferType
{
	Matrix world;
	Matrix view;
	Matrix projection;
};
