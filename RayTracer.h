#pragma once

#include "Sphere.h"
#include "Triangle.h"
#include "Ray.h"
#include "Light.h"
#include "Square.h"
#include "Texture.h"
#include <vector>

namespace slab
{
	using namespace std;
	using namespace glm;

	class Raytracer
	{
	public:
		int width, height;
		Light light;

		std::vector<shared_ptr<Object>> objects;
		unique_ptr<Texture> texture;

		Raytracer(const int& width, const int& height)
			: width(width), height(height)
		{
			auto sphere1 = make_shared<Sphere>(vec3(1.0f, 0.0f, 1.5f), 0.4f);
			sphere1->amb = vec3(0.2f);
			sphere1->diff = vec3(1.0f, 0.2f, 0.2f);
			sphere1->spec = vec3(0.5f);
			sphere1->alpha = 10.0f;

			objects.push_back(sphere1);

			auto square = make_shared<Square>(vec3(-2.0f, 2.0f, 2.0f), vec3(2.0f, 2.0f, 2.0f), vec3(2.0f, -2.0f, 2.0f), vec3(-2.0f, -2.0f, 2.0f),
				vec2(0.0f, 0.0f), vec2(1.0f, 0.0f), vec2(1.0f, 1.0f), vec2(0.0f, 1.0f));

			square->amb = vec3(1.0f);
			square->diff = vec3(0.0f);
			square->spec = vec3(0.0f);

			objects.push_back(square);


			light = Light{ {0.0f,1.0f,0.2f} }; // ȭ�� ����

			square->ambTexture = make_shared<Texture>(Texture("../Resources/shadertoy_abstract1.jpg"));
		}


		Hit FindColsestCollision(Ray& ray)
		{
			float closestD = 1000.0f; // ifnf
			Hit closest_hit = Hit{ -1.0f, dvec3(0.0f), dvec3(0.0f) };

			for (int l = 0; l < objects.size(); l++)
			{
				auto hit = objects[l]->CheckRayCollision(ray);

				if (hit.d >= 0.0f)
				{
					if (hit.d < closestD)
					{
						closestD = hit.d;
						closest_hit = hit;
						closest_hit.obj = objects[l];
					}
				}
			}

			return closest_hit;
		}

		// ������ ��ü�� ������ �� ��ü�� �� ��ȯ
		glm::vec3 traceRay(Ray& ray)
		{
			const Hit hit = FindColsestCollision(ray);

			if (hit.d >= 0.0f)
			{

				// ���⼭ �� �𵨷� ���� ȿ�� ���
				// https://en.wikipedia.org/wiki/Phong_reflection_model
				// https://www.scratchapixel.com/lessons/3d-basic-rendering/phong-shader-BRDF/phong-illumination-models-brdf.html
				glm::vec3 pointColor(hit.obj->amb);


				if (hit.obj->ambTexture)
				{
					pointColor = hit.obj->amb * hit.obj->ambTexture->SamplePoint(hit.uv);
				}
				else
				{
					pointColor = hit.obj->amb;
				}

				if (hit.obj->GetObjectType() == ObjectType::Square)
				{
					//pointColor = vec3(hit.uv,0);
				}

				// Diffuse
				glm::vec3 dirToLight = glm::normalize(light.pos - hit.point);
				float ndotl = glm::max(glm::dot(dirToLight, hit.normal), 0.0f);
				const float diff = ndotl;

				// Specular
				// const vec3 relfectDir = .. // r =2 (n dot l) n - l
				glm::vec3 reflectDir = glm::normalize(hit.normal * glm::dot(dirToLight, hit.normal) * 2.0f - dirToLight);
				const float specular = glm::pow(glm::max(glm::dot(-ray.dir, reflectDir), 0.0f), hit.obj->alpha);

				if (hit.obj->difTexture)
				{
					pointColor += diff * hit.obj->diff * hit.obj->difTexture->SamplePoint(hit.uv);
				}
				else
				{
					pointColor += diff * hit.obj->diff;
				}

				pointColor += hit.obj->spec * specular;

				return  pointColor;
			}

			return vec3(0.0f);
		}

		void Render(std::vector<glm::vec4>& pixels)
		{
			std::fill(pixels.begin(), pixels.end(), glm::vec4{ 0.0f,0.0f,0.0f,1.0f });

			const vec3 eyePos(0.0f, 0.0f, -1.5f);

#pragma omp parallel for
			for (int j = 0; j < height; j++)
				for (int i = 0; i < width; i++)
				{
					const glm::vec3 pixelPosWorld = TransformScreenToWorld(glm::vec2(i, j));

					// ������ ���� ����
					// ��ũ���� ������ z����, ���밪 1.0�� ���� ����
					// Orthographic projection (������) vs perspective projection (���� ����)
					const auto rayDir = glm::normalize(pixelPosWorld - eyePos);
					//const auto rayDir = glm::vec3(0.f, 0.f, 1.f);

					Ray pixelRay{ pixelPosWorld, rayDir };

					// index���� size_t�� ��� (index�� ������ ���� �����ϱ�)
					// traceRay()�� ��ȯ���� vec3 (RGB), A�� ���ʿ�
					pixels[size_t(i + width * j)] = glm::vec4(traceRay(pixelRay), 1.0f);
				}

		}

		glm::vec3 TransformScreenToWorld(glm::vec2 posScreen)
		{
			const float xScale = 2.0f / (width - 1);
			const float yScale = 2.0f / (height - 1);
			const float aspect = float(width) / height;

			// 3���� �������� Ȯ�� (z��ǥ�� 0.0���� ����)
			return glm::vec3((posScreen.x * xScale - 1.0f) * aspect, -posScreen.y * yScale + 1.0f, 0.0f);
		}

	};
}