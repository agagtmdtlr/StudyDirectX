#pragma once

#include <vector>
#include <glm/glm.hpp>

namespace slab
{
	class Image
	{
	public:
		float brightness = 1.01f;
		int width = 0, height = 0, channels = 0;
		std::vector<glm::vec4> pixels; // 이미지 처리할 때는 색을 float에 저장하는 것 이 더 정밀

		/*
		* 이미지 컨벌루션 참고 자료들
		* https://en.wikipedia.org/wiki/Kernel_(image_processing) // 마지막 괄호 주의
		* https://en.wikipedia.org/wiki/Convolution
		* https://medium.com/@bdhuma/6-basic-things-to-know-about-convolution-daef5e1bc411
		* https://towardsdatascience.com/intuitively-understanding-convolutions-for-deep-learning-1f6f42faee1
		*/

		void ReadFromFile(const char* filename);
		void WritePNG(const char* filename);
		glm::vec4& GetPixel(int i, int j);
		void BoxBlur5();
		void GaussianBlur5();
		void Bright();
		void Bloom(const float& th, const int& numRepeat, const float& weight = 1.0f);
	};
}

