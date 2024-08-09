#include "Example.h"

#include <omp.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb_image_write.h>

#include <algorithm> // std::clamp (C++17)

void Image::ReadFromFile(const char* filename)
{
    /*
    vcpkg install stb:x64-windows
    ������Ʈ �������� _CRT_SECURE_NO_WARNINGS �߰� ('sprintf' in stb_image_write.h)
    #define STB_IMAGE_IMPLEMENTATION
    #include <stb_image.h>
    #define STB_IMAGE_WRITE_IMPLEMENTATION
    #include <stb_image_write.h>
    */

    unsigned char* img = stbi_load(filename, &width, &height, &channels, 0);


    if (width)
    {
        std::cout << width << " " << height << " " << channels << std::endl;
    }
    else
    {
        std::cout << "Error : reading " << filename << " failed." << std::endl;
    }

    // channels�� 3(RGB) �Ǵ� 4(RGBA)�� ��츸 ����
    // unsigned char(0���� 255)�� 4ä�� float(0.0f���� 1.0f)�� ��ȭ
    pixels.resize(width * height);
    for (int i = 0; i < width * height; i++)
    {
        pixels[i].v[0] = img[i * channels] / 255.0f;
        pixels[i].v[1] = img[i * channels + 1] / 255.0f;
        pixels[i].v[2] = img[i * channels + 2] / 255.0f;
        pixels[i].v[3] = 1.0f;
    }

    delete[] img;
}

void Image::WritePNG(const char* filename)
{
    // 32bit���� 8bit�� ��ȯ �Ŀ� ����
    std::vector<unsigned char> img(width * height* channels, 0);
    for (int i = 0; i < width * height; i++)
    {
        img[i* channels] = uint8_t(pixels[i].v[0] * 255.0f); // v[0]�� 0.0f �̻� 1.0f ���� ����
        img[i* channels + 1] = uint8_t(pixels[i].v[1] * 255.0f);
        img[i * channels + 2] = uint8_t(pixels[i].v[2] * 255.0f);
    }

    stbi_write_png(filename,width, height, channels, img.data(), width * channels);
}

Vec4& Image::GetPixel(int i, int j)
{
    // TODO: ���⿡ return ���� �����մϴ�.
    i = std::clamp(i, 0, this->width -1);
    j = std::clamp(j, 0, this->height - 1);


    return pixels[j * this->width + i];
}

void Image::BoxBlur5()
{
    std::vector<Vec4> pixelsBuffer(this->pixels.size()); // �纻 ����
    
    
    /*
    Separable convolution
    �ѹ��� 2���� Kernel �� �����ϴ� ��ſ� 1���� Kernel�� �ι� ����
    �����ϱ� ���� ȿ�����̴�.
    */

    // ���� ���� (x ���� )
#pragma omp parallel for
    for (int j = 0; j < this->height; j++)
    {
        for (int i = 0; i < this->width; i++)
        {
            // �ֺ� �ȼ����� ���� ��ճ�� (i,j)�� �ִ� �ȼ��� ���� ����
            // this->pixels�κ��� �о�� ������ ��ճ�� pixelsBuffer�� ������ �ٲٱ�

            Vec4 kernelResult = {0.0f,0.0f,0.0f,0.0f};
            for (int c = 0; c < 5; c++)
            {
                int ci = std::clamp(i - 2, 0, this->width - 1);
                kernelResult += pixels[j * this->width + ci];
                kernelResult /= 5.0f;
            }

            pixelsBuffer[j * width + i] = kernelResult;
        }
    }

    // Swap
    std::swap(this->pixels, pixelsBuffer);

    return;     
}

void Image::GaussianBlur5()
{
}

void Image::Bloom(const float& th, const int& numRepeat, const float& weight)
{
}
