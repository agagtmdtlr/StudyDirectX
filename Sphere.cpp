#include "stdafx.h"
#include "Sphere.h"
#include "PrimitiveBuffer.h"
#include <math.h>


Sphere::Sphere()
{
	if (PrimitiveBufferManager::IsExisted(GetBufferName()) == false)
	{
		std::vector<Vertex> vertices;

		int count = 10;
		float r = 0.5f;
		float sectorStep = XM_2PI / (count);
		float stackStep = XM_PI / (count);
		float uvStep = 1.0f / (count);

		float sectorAngle, stackAngle;
		Vector4 pos;
		pos.w = 1.f;
		Vector2 uv;
		float sinStack, cosStack, sinSector, cosSector;

		for (int j = 0; j <= count; j++)
		{
			stackAngle = XM_PIDIV2 - j * stackStep;

			sinStack = sinf(stackAngle);
			cosStack = cosf(stackAngle);

			pos.y = r * sinStack;
			uv.y = uvStep * j;
			for (int i = 0; i <= count; i++)
			{
				sectorAngle = sectorStep * i;
				sinSector = sinf(sectorAngle);
				cosSector = cosf(sectorAngle);
				pos.z = r * cosStack * sinSector;
				pos.x = r * cosStack * cosSector;
				uv.x = uvStep * i;
				Vector3 normal(pos);
				normal.Normalize();
				vertices.push_back({ pos,normal });
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

				if (y != count - 1)
				{
					indices.push_back(k1 + 1);
					indices.push_back(k2 + 1);
					indices.push_back(k2);
				}
			}
		}

		shared_ptr<PrimitiveBuffer> buffer = make_unique<PrimitiveBuffer>(vertices, indices);
		PrimitiveBufferManager::RegistBuffer(GetBufferName(), buffer);

	}
	
	buffer = PrimitiveBufferManager::RequestBuffer(GetBufferName());
}

Sphere::~Sphere()
{
}

bool Sphere::RayCast(const Ray& ray)
{
	Vector3 p = transform.position;
	p -= ray.position;
	float b = p.Dot(ray.direction);
	if( b <= 0)
		return false;

	Vector3 o = (ray.direction * b) + ray.position;

	float l = (o - transform.position).Length();
	float r = 0.5f;
	return l <= r;
}




