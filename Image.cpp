#include "stdafx.h"
#include "Image.h"
#include <omp.h>
#include <iostream>
#include <memory>

#include "FileManager.h"

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
    unsigned char* img = FileManager::LoadImage(filename, &width, &height, &channels);
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
    std::vector<unsigned char> img(width * height * channels, 0);
    for (int i = 0; i < width * height; i++)
    {
        img[i * channels] = uint8_t(pixels[i].v[0] * 255.0f); // v[0]�� 0.0f �̻� 1.0f ���� ����
        img[i * channels + 1] = uint8_t(pixels[i].v[1] * 255.0f);
        img[i * channels + 2] = uint8_t(pixels[i].v[2] * 255.0f);
    }

    FileManager::WriteImage(filename, width, height, channels, img.data());
}

Vec4& Image::GetPixel(int i, int j)
{
    // TODO: ���⿡ return ���� �����մϴ�.
    i = std::clamp(i, 0, this->width - 1);
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
    https://en.wikipedia.org/wiki/Separable_filter
    https://towardsdatascience.com/a-basic-introduction-to-separable-convolutions-b99ec3102728
    */

    // ���� ���� (x ���� )
#pragma omp parallel for
    for (int j = 0; j < this->height; j++)
    {
        for (int i = 0; i < this->width; i++)
        {
            // �ֺ� �ȼ����� ���� ��ճ�� (i,j)�� �ִ� �ȼ��� ���� ����
            // this->pixels�κ��� �о�� ������ ��ճ�� pixelsBuffer�� ������ �ٲٱ�

            Vec4 kernelResult = { 0.0f,0.0f,0.0f,0.0f };
            for (int c = -2; c < 3; c++)
            {
                kernelResult += GetPixel(i + c, j);
            }

            kernelResult /= 5.0f; 
            pixelsBuffer[j * width + i] = kernelResult;
        }
    }

    // Swap
    std::swap(this->pixels, pixelsBuffer);


    // ���� ���� (y ���� )
#pragma omp parallel for
    for (int i = 0; i < this->width; i++)
    {
        for (int j = 0; j < this->height; j++)
        {
            // �ֺ� �ȼ����� ���� ��ճ�� (i,j)�� �ִ� �ȼ��� ���� ����
            // this->pixels�κ��� �о�� ������ ��ճ�� pixelsBuffer�� ������ �ٲٱ�

            Vec4 kernelResult = { 0.0f,0.0f,0.0f,0.0f };
            for (int c = -2; c < 3; c++)
            {
                kernelResult += GetPixel(i , j + c);
            }

            kernelResult /= 5.0f;
            pixelsBuffer[j * width + i] = kernelResult;
        }
    }

    // Swap
    std::swap(this->pixels, pixelsBuffer);

    return;
}

void Image::GaussianBlur5()
{
    std::vector<Vec4> pixelsBuffer(pixels.size());

    /*
    * �����ڷ�
    * https://en.wikipedia.org/wiki/Gaussian_filter
    * https://followtutorials.com/2013/03/gaussian-blurring-using-separable-kernel-in-c.html
    */
    const float weights[5] = {0.0545f, 0.2442f, 0.4026f, 0.2442f, 0.0545f};

    // ���� ����
#pragma omp parallel for
    for (int j = 0; j < this->height; j++)
    {
        for (int i = 0; i < this->width; i++)
        {
            Vec4 kernelResult = {0.f,0.f,0.f,0.f};
            for (int si = 0; si < 5; si++)
            {
                kernelResult += GetPixel(i + si - 2, j).Multiply(weights[si]);
            }

            pixelsBuffer[j * this->width + i] = kernelResult;            
        }
    }

    std::swap(this->pixels , pixelsBuffer);

    //���ι���
#pragma omp parallel for
    for (int i = 0; i < this->width; i++)
    {
        for (int j = 0; j < this->height; j++)
        {
            Vec4 kernelResult = {0.f,0.f,0.f,0.f};
            for (int si = 0; si < 5; si++)
            {
                kernelResult +=GetPixel(i,j +si -2).Multiply(weights[si]);
            }

            pixelsBuffer[j * this->width + i] = kernelResult;
        }
    }

    std::swap(this->pixels , pixelsBuffer);

    return;
}

void Image::Bright()
{
    std::vector<Vec4> pixelsBuffer(pixels.size());
    
#pragma omp parallel for
    for (int j = 0; j < height - 1; j++)
    {
        for (int i = 0; i < width - 1; i++)
        {
            float inverseBrightness = i < width / 2 ? 0.99f : 1.01f;

            pixelsBuffer[j * width + i] = pixels[j* width + i].Multiply(inverseBrightness).Clamp(0.0f,1.0f);
            pixelsBuffer[j * width + i].v[3] = 1.0f;
        }
    }

    std::swap(this->pixels, pixelsBuffer);

}

void Image::Bloom(const float& th, const int& numRepeat, const float& weight)
{
    //https://learnopengl.com/Advanced-Lighting/Bloom
    const std::vector<Vec4> pixelsBackup = this->pixels; // �޸� ���빰���� ��� ����

    /* Brightness�� threshold ���� ���� �ȼ� ���� ��� ���������� �ٲٱ�
    *  https://en.wikipedia.org/wiki/Relative_luminance
    *  Relative Luminance Y = 0.2126*R + 0.7152*G + 0.0722*B
    */
    for (int j = 0; j < height; j++)
    {
        for (int i = 0; i < width; i++)
        {
            float relativeLuminanceY = GetPixel(i,j).Dot(Vec4{0.2126f,0.7152f,0.0722f,1.f});
            if (relativeLuminanceY < th)
                pixels[j*width + i] = Vec4{0.f,0.f,0.f,1.f};
        }
    }

    // ���� �κи� Blur
    for (int i = 0; i < numRepeat; i++)
    {
        GaussianBlur5();
    }

    // ���� �κи� Blur�� �Ͱ� ���� �̹����� ���ϱ� (���� �κ� Blur�� weight ���ؼ� ���� ����)
    for (int i = 0; i < pixelsBackup.size(); i++)
    {
        pixels[i] = pixels[i].Multiply(weight).Add(pixelsBackup[i]).Clamp(0.f,1.f);        
    }

    return;   

}
