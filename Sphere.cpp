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
		| \  |
		|  \ |
		k3---k4
		*/

		for (int j = 0; j < count; j++)
		{
			UINT k1 = j * (count+1);
			UINT k2 = k1 + count + 1;
			for (int i = 0; i < count; i++, ++k1, ++k2)
			{
				if (j != 0)
				{
					indices.push_back(k1);
					indices.push_back(k1+1);
					indices.push_back(k2);
				}

				if (j != count - 1)
				{
					indices.push_back(k1+1);
					indices.push_back(k2+1);
					indices.push_back(k2);
				}


			}
		}

		//int last = vertices.size();
		//for (int i = 0; i < count; i++)
		//{
		//	int off = last - 1 - count + i;
		//	indices.push_back(off);
		//	indices.push_back(off + 1);
		//	indices.push_back(last);
		//}


		buffer = make_unique<PrimitiveBuffer>(vertices, indices);


	}
}

PrimitiveBuffer* Sphere::GetBuffer()
{
	return buffer.get();
}

UINT Sphere::GetIndexCount()
{
	return buffer->GetIndexCount();
}
