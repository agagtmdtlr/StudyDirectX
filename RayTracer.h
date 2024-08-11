#pragma once

#include "Sphere.h"
#include "Ray.h"
#include "Light.h"

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

		Raytracer(const int& width, const int& height)
			: width(width), height(height)
		{
			auto sphere1 = make_shared<Sphere>(vec3(0.5f, 0.0f, 0.5f), 0.4f, vec3(0.5f, 0.5f, 0.5f));
			auto sphere2 = make_shared<Sphere>(vec3(0.0f, 0.0f, 1.0f), 0.4f, vec3(0.5f, 0.5f, 0.5f));
			auto sphere3 = make_shared<Sphere>(vec3(-0.5f, 0.0f, 1.5f), 0.4f, vec3(0.5f, 0.5f, 0.5f));

			sphere1->amb = glm::vec3(0.2f);
			sphere1->diff = glm::vec3(1.0f, 0.2f, 0.2f);
			sphere1->spec = glm::vec3(0.5f);
			sphere1->alpha = 10.0f;

			sphere2->amb = glm::vec3(0.2f);
			sphere2->diff = glm::vec3(0.2f, 1.0f, 0.2f);
			sphere2->spec = glm::vec3(0.5f);
			sphere2->alpha = 10.0f;

			sphere3->amb = glm::vec3(0.2f);
			sphere3->diff = glm::vec3(0.2f, 0.2f, 1.0f);
			sphere3->spec = glm::vec3(0.5f);
			sphere3->alpha = 10.0f;

			// 일부러 역순으로 추가
			objects.push_back(sphere3);
			objects.push_back(sphere2);
			objects.push_back(sphere1);


			light = Light{ {0.0f,1.0f,-1.0f} }; // 화면 뒷쪽
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

		// 광선이 물체에 닿으면 그 물체의 색 반환
		glm::vec3 traceRay(Ray& ray)
		{
			const Hit hit = FindColsestCollision(ray);

			if (hit.d < 0.0f)
			{
				return glm::vec3(0.0f);
			}
			else
			{
				glm::vec3 eye = ray.start;
				//return sphere->color * hit.d; // 깊이를 곱해서 입체감 만들기

				// 여기서 퐁 모델로 조면 효과 계산
				// 참고 자료
				// https://en.wikipedia.org/wiki/Phong_reflection_model
				// https://www.scratchapixel.com/lessons/3d-basic-rendering/phong-shader-BRDF/phong-illumination-models-brdf.html
				// 

				// Diffuse
				// const vec3 dirToLight
				glm::vec3 dirToLight = glm::normalize(light.pos - hit.point);
				float ndotl = glm::max(glm::dot(dirToLight, hit.normal), 0.0f);
				const float diff = ndotl;

				// Specular
				// const vec3 relfectDir = .. // r =2 (n dot l) n - l
				glm::vec3 reflectDir = glm::normalize(hit.normal * glm::dot(dirToLight, hit.normal) * 2.0f - dirToLight);

				const float specular = glm::pow(glm::max(glm::dot(-ray.dir, reflectDir), 0.0f), hit.obj->alpha);

				//return sphere->amb + sphere->diff * diff;
				return hit.obj->amb + hit.obj->diff * diff + hit.obj->spec * specular;
			}
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

					// 광선의 방향 벡터
					// 스크린에 수직인 z방향, 절대값 1.0인 유닛 벡터
					// Orthographic projection (정투영) vs perspective projection (원근 투영)

					const auto rayDir = glm::normalize( pixelPosWorld - eyePos );
					//const auto rayDir = glm::vec3(0.f, 0.f, 1.f);

					Ray pixelRay{ pixelPosWorld, rayDir };

					// index에는 size_t형 사용 (index가 음수일 수는 없으니까)
					// traceRay()의 반환형은 vec3 (RGB), A는 불필요
					pixels[size_t(i + width * j)] = glm::vec4(traceRay(pixelRay), 1.0f);
				}

		}

		glm::vec3 TransformScreenToWorld(glm::vec2 posScreen)
		{
			const float xScale = 2.0f / (width - 1);
			const float yScale = 2.0f / (height - 1);
			const float aspect = float(width) / height;

			// 3차원 공간으로 확장 (z좌표는 0.0으로 가정)
			return glm::vec3((posScreen.x * xScale - 1.0f) * aspect, -posScreen.y * yScale + 1.0f, 0.0f);
		}

	};
}