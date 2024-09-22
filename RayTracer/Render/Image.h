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
		std::vector<glm::vec4> pixels; // �̹��� ó���� ���� ���� float�� �����ϴ� �� �� �� ����

		/*
		* �̹��� ������� ���� �ڷ��
		* https://en.wikipedia.org/wiki/Kernel_(image_processing) // ������ ��ȣ ����
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

