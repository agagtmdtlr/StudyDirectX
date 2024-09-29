#include "stdafx.h"
#include "Sphere.h"
#include "PrimitiveBuffer.h"
#include <math.h>

std::unique_ptr<struct PrimitiveBuffer> Sphere::buffer = nullptr;


Sphere::Sphere()
{
	if (buffer == nullptr)
	{
		std::vector<Vertex> vertices;

		int count = 20;
		float r = 0.5f;
		float sectorStep = XM_2PI / count;
		float stackStep = XM_PI / count;
		float uvStep = 1.0f / count;

		float sectorAngle, stackAngle;
		Vector4 pos;
		Vector2 uv;
		float sinStack, cosStack, sinSector, cosSector;

		for (int j = 0; j < count; j++)
		{
			stackAngle = XM_PIDIV2 - j * stackStep;

			sinStack = sinf(stackAngle);
			cosStack = cosf(stackAngle);
			
			pos.y = r * sinStack;
			uv.y = uvStep * j;
			for (int i = 0; i < count; i++)
			{
				sectorAngle = sectorStep * i;
				sinSector = sinf(sectorAngle);
				cosSector = cosf(sectorAngle);
				pos.z = r * cosStack * sinSector;
				pos.x = r * cosStack * cosSector;
				uv.x = uvStep * i;

				vertices.push_back({pos,uv});
			}
		}

		std::vector<UINT> indices;
		for (int i = 0; i < count; i++)
		{
			indices.push_back(i % count + 1);
			indices.push_back(0);
			indices.push_back((i + 1) % count + 1);
		}

		/*
		clockwise front face
		k->k1->k2 / k1->k3->k2
		k ---k1
		|  / |
		| /  |	
		k2---k3
		*/

		for (int j = 1; j < count ; j++)
		{	
			for (int i = 0; i < count; i++)
			{
				UINT k2 = count * j + i;
				UINT k3 = count * j + ( (i+1) % count );
				UINT k = k2 - count;
				UINT k1 = k3 - count;
				indices.push_back(k);
				indices.push_back(k1);
				indices.push_back(k2);

				indices.push_back(k1);
				indices.push_back(k3);
				indices.push_back(k2);
			}
		}

		buffer = make_unique<PrimitiveBuffer>(vertices, indices);


	}
}

PrimitiveBuffer* Sphere::GetBuffer()
{
	return buffer.get();
}
