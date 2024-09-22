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

		int supersmaplingLevel = 0;

		Raytracer(const int& width, const int& height)
			: width(width), height(height)
		{
			auto sphere1 = make_shared<Sphere>(vec3(0.0f, -0.1f, 1.5f), 1.0f);
			sphere1->amb = vec3(0.1f);
			sphere1->diff = vec3(1.0f, 0.0f, 0.0f);
			sphere1->spec = vec3(1.0f);
			sphere1->alpha = 10.0f;
			sphere1->reflection = 0.0f;
			sphere1->transparency = 1.0f;
			objects.push_back(sphere1);

			auto sphere2 = make_shared<Sphere>(vec3(1.2f, -0.1f, 0.5f), 0.4f);
			sphere2->amb = vec3(0.0f);
			sphere2->diff = vec3(0.0f, 0.0f, 1.0f);
			sphere2->spec = vec3(1.0f);
			sphere2->alpha = 50.0f;
			sphere2->reflection = 0.0f;
			objects.push_back(sphere2);

			auto groundTexture = std::make_shared<Texture>("shadertoy_abstract1.jpg");
			auto ground = make_shared<Square>(vec3(-10.0f, -1.2f, 0.0f), vec3(-10.0f, -1.2f, 10.0f), vec3(10.0f, -1.2f, 10.0f), vec3(10.0f, -1.2f, 0.0f),
				vec2(0.0f, 0.0f), vec2(1.0f, 0.0f), vec2(1.0f, 1.0f), vec2(0.0f, 1.0f));
			ground->amb = vec4(1.0f);
			ground->diff = vec4(1.0f);
			ground->spec = vec4(1.0f);
			ground->alpha = 10.0f;
			ground->reflection = 0.0f;
			ground->ambTexture = groundTexture;
			ground->difTexture = groundTexture;

			objects.push_back(ground);

			auto squareTexture = std::make_shared<Texture>("back.jpg");
			auto square = make_shared<Square>(vec3(-10.0f, 10.0f, 10.0f), vec3(10.0f, 10.0f, 10.0f), vec3(10.0f, -10.0f, 10.0f), vec3(-10.0f, -10.0f, 10.0f),
				vec2(0.0f, 0.0f), vec2(1.0f, 0.0f), vec2(1.0f, 1.0f), vec2(0.0f, 1.0f));

			square->amb = vec3(1.0f);
			square->diff = vec3(0.0f);
			square->spec = vec3(0.0f);
			square->alpha = 10.0f;
			square->reflection = 0.0f;
			square->ambTexture = squareTexture;
			square->difTexture = squareTexture;

			objects.push_back(square);

			light = Light{ {0.0f, 0.3f, -0.5f} }; // 화면 뒷쪽

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

						closest_hit.uv = hit.uv;
					}
				}
			}

			return closest_hit;
		}

		vec3 refract(const vec3& I, const vec3& N, const float& ior)
		{
			float eta = ior;

			vec3 n = N;
			if (dot(-I, N) < 0) // in - out
			{
				eta = 1/eta;
				n = -n;
			}

			const float cos1 = dot(-I, n);
			const float sin1 = sqrt(1.f - cos1 * cos1);
			const float sin2 = sin1 / eta;
			const float cos2 = sqrt(1.f - sin2 * sin2);

			const vec3 m = glm::normalize( I + n * cos1);
			return m * sin2 + -n * cos2;
		}

		// 광선이 물체에 닿으면 그 물체의 색 반환
		glm::vec3 traceRay(Ray& ray, const int recursiveLevel)
		{
			if (recursiveLevel < 0)
				return vec3(0.0f);

			const Hit hit = FindColsestCollision(ray);

			if (hit.d >= 0.0f)
			{

				// 여기서 퐁 모델로 조면 효과 계산
				// https://en.wikipedia.org/wiki/Phong_reflection_model
				// https://www.scratchapixel.com/lessons/3d-basic-rendering/phong-shader-BRDF/phong-illumination-models-brdf.html
				glm::vec3 color(0.0f);
				glm::vec3 dirToLight = glm::normalize(light.pos - hit.point);


				{
					glm::vec3 phongColor(0.0f);
					// Diffuse
					float ndotl = glm::max(glm::dot(dirToLight, hit.normal), 0.0f);
					const float diff = ndotl;
					glm::vec3 reflectDir = hit.normal * glm::dot(dirToLight, hit.normal) * 2.0f - dirToLight;
					const float specular = glm::pow(glm::max(glm::dot(-ray.dir, reflectDir), 0.0f), hit.obj->alpha);

					if (hit.obj->ambTexture)
					{
						phongColor = hit.obj->amb * hit.obj->ambTexture->SampleLineaer(hit.uv);
					}
					else
					{
						phongColor = hit.obj->amb;
					}

					if (hit.obj->difTexture)
					{
						phongColor += diff * hit.obj->diff * hit.obj->difTexture->SampleLineaer(hit.uv);
					}
					else
					{
						phongColor += diff * hit.obj->diff;
					}

					phongColor += hit.obj->spec * specular;

					// 반사가 되는 양, 투과되는 양
					color += phongColor * (1.0f - hit.obj->reflection - hit.obj->transparency);

					if (hit.obj->reflection)
					{
						// 수치 오류 주의
						// 반사광이 반환해준 색을 더할 때의 비율은 hit.obj->reflection

						const auto reflectedDirection = glm::normalize(hit.normal * 2.0f * glm::dot(hit.normal, -ray.dir) + ray.dir);
						Ray reflectRay = { hit.point + reflectedDirection * 1e-4f, reflectedDirection };

						color += traceRay(reflectRay, recursiveLevel - 1) * hit.obj->reflection;
					}

					// 참고
					// https://samdriver.xyz/article/refraction-sphere (그림들이 좋아요)
					// https://www.scratchapixel.com/lessons/3d-basic-rendering/introduction-to-shading/reflection-refraction-fresnel (오류있음)
					// https://web.cse.ohio-state.edu/~shen.94/681/Site/Slides_files/reflection_refraction.pdf (슬라이드가 보기 좋지는 않지만 정확해요)
					if (hit.obj->transparency)
					{
						const float ior = 1.5f; // index of refraction ( glass : 1.5 , water : 1.3 )

						vec3 normal = hit.normal;
						float eta;

						float etai = ior, etat = 1;
						if(glm::dot(ray.dir, hit.normal) < 0.0f) // 밖에서 안으로 들어가는 경우 (ex) air->glass
						{ 
							eta = ior;
							normal = hit.normal;
						}
						else // 안에서 밖으로 나가는 경우 (ex) glass->air
						{
							eta = 1.0f / ior;
							normal = -hit.normal;
						}

						const float cosTheta1 = glm::dot(-ray.dir, normal);
						const float sinTheta1 = sqrt(1.0f - cosTheta1 * cosTheta1); // cos^2 + sin^2 = 1
						const float sinTheta2 = sinTheta1 / eta;
						const float cosTheta2 = sqrt(1.0f - sinTheta2 * sinTheta2);


						// k < 0.0f 이란 뜻 굴절 임계값을 번어사 굴절이 아닌 반사가 일어난 것이다
						const vec3 m = glm::normalize(ray.dir + normal * cosTheta1);
						const vec3 A = m * sinTheta2;
						const vec3 B = -normal * cosTheta2;
						const vec3 refractedDirection = glm::normalize(A + B); // transmission


						//if (k >= 0.0f)
						{
							Ray refectRay = { hit.point + refractedDirection * 1e-4f, refractedDirection };
							color += traceRay(refectRay, recursiveLevel - 1) * hit.obj->transparency;
						}
						
						
						// Fresnel 효과는 생략되었습니다.

					}


					//Ray shadowRay = { hit.point + dirToLight * 1e-4f, dirToLight };
					//Hit shadowHit = FindColsestCollision(shadowRay);
					//if (shadowHit.d >= 0.0f)
					//{
					//	color *= 0.5f;
					//}
				}


				return  color;
			}

			return vec3(0.0f);
		}

		glm::vec3 traceRay2x2(vec3 eyePos, vec3 pixelPos, const float dx, const int recursiveLevel)
		{
			if (recursiveLevel == 0)
			{
				Ray ray{ pixelPos, glm::normalize(pixelPos - eyePos) };
				return traceRay(ray, 0);
			}
			/*
			dx
			0  1  2  3  4
			_____________
			|__|a_|__|b_|
			|__|__|p_|__|
			|__|c_|__|d_|
			|__|__|__|__|
			------
			dx 0-4
			subdx 0-2
			c = vec2( p.x - subdx * 0.5 , p.y - subdx * 0.5 )

			*/

			const float subdx = 0.5f * dx;

			vec3 pixelColor(0.0f);
			// 
			pixelPos = vec3(pixelPos.x - subdx * 0.5f, pixelPos.y - subdx * 0.5f, pixelPos.z);
			//[수정] 강의 영상과 달리 subdx에 0.5f를 곱해줬스니다
			const float subddx = subdx;
			for (int j = 0; j < 2; j++)
			{
				for (int i = 0; i < 2; i++)
				{
					vec3 subPos(pixelPos.x + float(i) * subdx, pixelPos.y + float(j) * subdx, pixelPos.z);
					pixelColor += traceRay2x2(eyePos, subPos, subdx, recursiveLevel - 1);
				}
			}
			pixelColor += traceRay2x2(eyePos, pixelPos, subdx, recursiveLevel - 1);
			pixelColor += traceRay2x2(eyePos, pixelPos + vec3(subddx, 0.0f, 0.0f), subdx, recursiveLevel - 1);
			pixelColor += traceRay2x2(eyePos, pixelPos + vec3(0, subddx, 0.0f), subdx, recursiveLevel - 1);
			pixelColor += traceRay2x2(eyePos, pixelPos + vec3(subddx, subddx, 0.0f), subdx, recursiveLevel - 1);

			return pixelColor * 0.25f;
		}

		void Render(std::vector<glm::vec4>& pixels)
		{
			std::fill(pixels.begin(), pixels.end(), glm::vec4{ 0.0f,0.0f,0.0f,1.0f });

			const vec3 eyePos(0.0f, 0.0f, -1.5f);

			const float dx = 2.0f / this->height;

			const int recursiveLevel = 3;


#pragma omp parallel for
			for (int j = 0; j < height; j++)
				for (int i = 0; i < width; i++)
				{
					// 광선의 방향 벡터
					// 스크린에 수직인 z방향, 절대값 1.0인 유닛 벡터
					// Orthographic projection (정투영) vs perspective projection (원근 투영)
					glm::vec3 pixelPosWorld = TransformScreenToWorld(glm::vec2(i, j));
					const auto rayDir = glm::normalize(pixelPosWorld - eyePos);

					Ray pixelRay{ pixelPosWorld, rayDir };

					// index에는 size_t형 사용 (index가 음수일 수는 없으니까)
					// traceRay()의 반환형은 vec3 (RGB), A는 불필요
					pixels[i + width * j] = vec4(glm::clamp(traceRay(pixelRay, 5), 0.0f, 1.0f), 1.0f);

					/* super sampling */
					//const auto pixelColor = traceRay2x2(eyePos, pixelPosWorld, dx, supersmaplingLevel);
					//pixels[size_t(i + width * j)] = vec4(glm::clamp(pixelColor, 0.0f, 1.0f), 1.0f);
				}

		}

		glm::vec3 TransformScreenToWorld(glm::vec2 posScreen)
		{
			const float xScale = 2.0f / float(width);
			const float yScale = 2.0f / float(height);
			const float aspect = float(width) / height;

			// 3차원 공간으로 확장 (z좌표는 0.0으로 가정)
			return glm::vec3((posScreen.x * xScale - 1.0f) * aspect, -posScreen.y * yScale + 1.0f, 0.0f);
		}

	};
}