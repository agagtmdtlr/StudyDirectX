#pragma once

#include "Sphere.h"
#include "Triangle.h"
#include "Ray.h"
#include "Light.h"
#include "Square.h"

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

		shared_ptr<Sphere> sphere;
		std::vector<shared_ptr<Object>> objects;



		Raytracer(const int& width, const int& height)
			: width(width), height(height)
		{
			auto sphere1 = make_shared<Sphere>(vec3(0.0f, 0.0f, 0.6f), 0.4f);
			sphere1->amb = vec3(0.2f, 0.0f, 0.0f);
			sphere1->diff = vec3(1.0f, 0.1f, 0.1f);
			sphere1->spec = vec3(1.5f);
			sphere1->alpha = 50.0f;
			
			this->sphere = sphere1;
			objects.push_back(sphere1);

			auto triangle1 = make_shared<Triangle>(vec3(-2.0f, -1.0f, 0.0f), vec3(-2.0f, -1.0f, 4.0f), vec3(2.0f, -1.0f, 4.0f));
			auto triangle2 = make_shared<Triangle>(vec3(-2.0f, -1.0f, 0.0f), vec3(2.0f, -1.0f, 4.0f), vec3(2.0f, -1.0f, 0.0f));
			triangle1->amb = vec3(0.2f);
			triangle1->diff = vec3(0.8f);
			triangle1->spec = vec3(1.0f);
			triangle1->alpha = 50.0f;
			triangle2->amb = vec3(0.2f);
			triangle2->diff = vec3(0.8f);
			triangle2->spec = vec3(1.0f);
			triangle2->alpha = 50.0f;

			objects.push_back(triangle1);
			objects.push_back(triangle2);

			//auto quad1 = make_shared<Quad>(vec3(-2.0f, -2.0f, 0.0f), vec3(-2.0f, 2.0f, 10.0f), vec3(2.0f, 2.0f, 10.0f), vec3(2.0f, -2.0f, 0.0f));
			//quad1->amb = vec3(0.2f);
			//quad1->diff = vec3(0.5f);
			//quad1->spec = vec3(0.5f);
			//quad1->alpha = 5.0f;
			//objects.push_back(quad1);


			light = Light{ {0.0f,1.0f,0.2f} }; // ȭ�� ����
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
				// ���� �ڷ�
				// https://en.wikipedia.org/wiki/Phong_reflection_model
				// https://www.scratchapixel.com/lessons/3d-basic-rendering/phong-shader-BRDF/phong-illumination-models-brdf.html
				// 
				glm::vec3 color(hit.obj->amb);

				vec3 dirToLight = normalize( light.pos - hit.point );
				Ray shadowRay = Ray{hit.point + dirToLight * 1e-4f, dirToLight};
				if (FindColsestCollision(shadowRay).d < 0.0f)
				{
					// Diffuse
					// const vec3 dirToLight
					glm::vec3 dirToLight = glm::normalize(light.pos - hit.point);
					float ndotl = glm::max(glm::dot(dirToLight, hit.normal), 0.0f);
					const float diff = ndotl;

					// Specular
					// const vec3 relfectDir = .. // r =2 (n dot l) n - l
					glm::vec3 reflectDir = glm::normalize(hit.normal * glm::dot(dirToLight, hit.normal) * 2.0f - dirToLight);

					const float specular = glm::pow(glm::max(glm::dot(-ray.dir, reflectDir), 0.0f), hit.obj->alpha);

					color += hit.obj->diff * diff + hit.obj->spec * specular;
				}
				return  color;
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