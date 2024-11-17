#include "stdafx.h"
#include "Sphere.h"
#include "PrimitiveBuffer.h"
#include <math.h>


Sphere::Sphere()
{
}

Sphere::~Sphere()
{
}

void Sphere::LoadMesh(string path)
{
	string bufferName = "sphere";
	if (PrimitiveBufferManager::IsExisted(bufferName) == false)
	{
		std::vector<Vertex> vertices;

		int count = 36;
		float r = 0.5f;
		float sectorStep = XM_2PI / (count);
		float stackStep = XM_PI / (count);
		float uvStep = 1.0f / (count);

		float sectorAngle, stackAngle;
		Vector3 pos;
		Vector2 uv;
		float sinStack, cosStack, sinSector, cosSector;

		for (int y = 0; y <= count; y++)
		{
			stackAngle = XM_PIDIV2 - y * stackStep;
			float xy = r * cosf(stackAngle);
			pos.y = r * sinf(stackAngle);
			uv.y = uvStep * y;
			for (int x = 0; x <= count; x++)
			{
				sectorAngle = sectorStep * x;
				pos.z = xy * sinf(sectorAngle);
				pos.x = xy * cosf(sectorAngle);
				uv.x = uvStep * x;
				Vector3 normal(pos);
				normal.Normalize();
				vertices.push_back({ pos,normal ,uv });
			}
		}

		std::vector<UINT> indices;
		/*
		k1---k2
		|  / |
		| /  |
		k3---k4
		*/

		for (int y = 0; y < count; y++)
		{
			UINT k1 = y * (count + 1);
			UINT k2 = k1 + count + 1;
			for (int x = 0; x < count; x++, ++k1, ++k2)
			{
				if (y != 0)
				{
					indices.push_back(k1);
					indices.push_back(k1 + 1);
					indices.push_back(k2);
				}

				if (y != (count - 1))
				{
					indices.push_back(k1 + 1);
					indices.push_back(k2 + 1);
					indices.push_back(k2);
				}
			}
		}

		for (auto i : indices)
		{
			if (i >= vertices.size())
			{
				assert(false);
			}
		}

		PrimitiveBufferManager::RegistBuffer(bufferName, vertices, indices);

	}
	buffer = PrimitiveBufferManager::RequestBuffer(bufferName);
}

bool Sphere::RayCast(const DirectX::SimpleMath::Ray& ray)
{
	Vector3 p = transform.position;
	p -= ray.position;
	float b = p.Dot(ray.direction);
	if (b <= 0)
		return false;

	Vector3 o = (ray.direction * b) + ray.position;

	float l = (o - transform.position).Length();
	float r = 0.5f;
	return l <= r;
}

Cube::Cube()
{
}

void Cube::LoadMesh(string path)
{
	string bufferName = "cube";
	if (PrimitiveBufferManager::IsExisted(bufferName) == false)
	{
		std::vector<Vertex> vertices;
		// back
		vertices.push_back({ Vector3(-0.5,-0.5,-0.5),Vector3(0,0,-1), Vector2(0,1) });
		vertices.push_back({ Vector3(-0.5,+0.5,-0.5),Vector3(0,0,-1), Vector2(0,0) });
		vertices.push_back({ Vector3(+0.5,+0.5,-0.5),Vector3(0,0,-1), Vector2(1,0) });
		vertices.push_back({ Vector3(+0.5,-0.5,-0.5),Vector3(0,0,-1), Vector2(1,1) });

		// front
		vertices.push_back({ Vector3(+0.5,-0.5,+0.5),Vector3(0,0,+1), Vector2(0,1) });
		vertices.push_back({ Vector3(+0.5,+0.5,+0.5),Vector3(0,0,+1), Vector2(0,0) });
		vertices.push_back({ Vector3(-0.5,+0.5,+0.5),Vector3(0,0,+1), Vector2(1,0) });
		vertices.push_back({ Vector3(-0.5,-0.5,+0.5),Vector3(0,0,+1), Vector2(1,1) });

		// left
		vertices.push_back({ Vector3(-0.5,-0.5,+0.5),Vector3(-1,0,0), Vector2(0,1) });
		vertices.push_back({ Vector3(-0.5,+0.5,+0.5),Vector3(-1,0,0), Vector2(0,0) });
		vertices.push_back({ Vector3(-0.5,+0.5,-0.5),Vector3(-1,0,0), Vector2(1,0) });
		vertices.push_back({ Vector3(-0.5,-0.5,-0.5),Vector3(-1,0,0), Vector2(1,1) });
		
		// right
		vertices.push_back({ Vector3(+0.5,-0.5,-0.5),Vector3(+1,0,0), Vector2(0,1) });
		vertices.push_back({ Vector3(+0.5,+0.5,-0.5),Vector3(+1,0,0), Vector2(0,0) });
		vertices.push_back({ Vector3(+0.5,+0.5,+0.5),Vector3(+1,0,0), Vector2(1,0) });
		vertices.push_back({ Vector3(+0.5,-0.5,+0.5),Vector3(+1,0,0), Vector2(1,1) });

		// up
		vertices.push_back({ Vector3(-0.5,+0.5,-0.5),Vector3(0,+1,0), Vector2(0,1) });
		vertices.push_back({ Vector3(-0.5,+0.5,+0.5),Vector3(0,+1,0), Vector2(0,0) });
		vertices.push_back({ Vector3(+0.5,+0.5,+0.5),Vector3(0,+1,0), Vector2(1,0) });
		vertices.push_back({ Vector3(+0.5,+0.5,-0.5),Vector3(0,+1,0), Vector2(1,1) });
	
		// down
		vertices.push_back({ Vector3(+0.5,-0.5,-0.5),Vector3(0,-1,0), Vector2(0,1) });
		vertices.push_back({ Vector3(+0.5,-0.5,+0.5),Vector3(0,-1,0), Vector2(0,0) });
		vertices.push_back({ Vector3(-0.5,-0.5,+0.5),Vector3(0,-1,0), Vector2(1,0) });
		vertices.push_back({ Vector3(-0.5,-0.5,-0.5),Vector3(0,-1,0), Vector2(1,1) });

		/*
		k1---k2
		|  / |
		| /  |
		k0---k3
		*/
		int count = vertices.size();
		std::vector<UINT> indices;
		for (int v = 0; v < count; v+=4)
		{
			indices.push_back(v + 0);
			indices.push_back(v + 1);
			indices.push_back(v + 2);

			indices.push_back(v + 2);
			indices.push_back(v + 3);
			indices.push_back(v + 0);
		}
		PrimitiveBufferManager::RegistBuffer(bufferName, vertices, indices);

	}
	buffer = PrimitiveBufferManager::RequestBuffer(bufferName);
}
