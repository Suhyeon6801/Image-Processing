#pragma once
#define _CRT_SECURE_NO_WARNINGS

#include<stdio.h>
#include<iostream>
#include<stdlib.h>
#include<string.h>//memset�Լ� ���
#define height_in 256
#define width_in 256

using namespace std;

class HW
{
private:
	double x; //scaling �Է°�
	//double height;
	//double width;
	unsigned char** Image_In;
	unsigned char** Image_Out;
	int height_Scale;
	int width_Scale;

public:
	HW();
	//HW(int h, int w);
	~HW();
	void ImageOpen();
	void ImageScaling();
	unsigned char BilinearInterpolation(int height_Ori, int width_Ori, double h_Cvt, double w_Cvt);
	void ImageSave();
};

HW::HW()
{

}
/*
HW::HW(int h, int w)
{
	height = h;
	width = w;
}*/

HW::~HW() {}

void HW::ImageOpen()
{
	FILE* pFile;
	pFile = fopen("lena256.raw", "rb");
	//char Image_arr[height_in][width_in];

	//�޸� �Ҵ�
	Image_In = new unsigned char*[width_in];
	for (int i = 0; i < height_in; ++i)
	{
		Image_In[i] = new unsigned char[height_in];
	}

	//�̹��� �о����
	if (pFile == NULL) // ����ó��
	{
		cout << "File not found!\n";
		return;
	}
	//fread(Image_In, sizeof(unsigned char), height_in*width_in, pFile);

	for (int i = 0; i < height_in; i++)
	{
		fread((void*)Image_In[i], sizeof(unsigned char), width_in, pFile);
	}
	fclose(pFile);

}

void HW::ImageScaling()
{
	cout << "Scaling Value : ";
	cin >> x;

	height_Scale = (floor)(height_in * x);
	width_Scale = (floor)(width_in * x);

	//�Է¹��� x���� ũ���� �迭 ���� �� �Ҵ�
	Image_Out = new unsigned char*[width_Scale];
	for (int i = 0; i < width_Scale; ++i)
	{
		Image_Out[i] = new unsigned char[height_Scale];
		memset(Image_Out[i], 0, sizeof(unsigned char)*height_Scale);//�޸� ������ 0�� �ʱ�ȭ
	}

	//Out�迭�� In�迭 Scaling
	for (int i = 0; i < height_Scale; i++)
	{
		for (int j = 0; j < width_Scale; j++)
		{
			double h_Ori = i / x;
			double w_Ori = j / x;
			Image_Out[i][j] = BilinearInterpolation(height_in, width_in, h_Ori, w_Ori);
			//cout << Image_Out[i][j];
		}
	}
}

unsigned char HW::BilinearInterpolation(int height_Ori, int width_Ori, double h_Cvt, double w_Cvt)
{
	int h1 = floor(h_Cvt);
	int w1 = floor(w_Cvt);

	double d1 = w_Cvt - w1;
	double d2 = 1 - d1;
	double d3 = h_Cvt - h1;
	double d4 = 1 - d3;

	unsigned char p1, p2, p3, p4;
	unsigned char pixel_val;//��ǥ�� pixel��

	if (h1 == height_Ori - 1 && w1 == width_Ori - 1)
	{
		return (unsigned char)Image_In[h1][w1];
	}
	else if (h1 == height_Ori - 1 || w1 != width_Ori - 1)
	{
		return (unsigned char)Image_In[h1][w1];
	}
	else if (h1 != height_Ori - 1 || w1 == width_Ori - 1)
	{
		return (unsigned char)Image_In[h1][w1];
	}
	else
	{
		p1 = Image_In[h1][w1];//(0,0)
		p2 = Image_In[h1][w1 + 1];//(1,0)
		p3 = Image_In[h1 + 1][w1];//(0,1)
		p4 = Image_In[h1 + 1][w1 + 1];//(1,1)

		pixel_val = ((p1*d2 + p2 + d1)*d4) + ((p3*d2 + p4 * d1)*d3);
		return (unsigned char)pixel_val;
	}
}

void HW::ImageSave()
{
	FILE *pFile;
	pFile = fopen("lena256_scaling.raw", "wb");
	/**
	if (pFile == NULL)
	{
		cout << "Fail to read file";
		return;
	}*/

	for (int i = 0; i < height_Scale; i++)
	{
		fwrite((const void*)Image_Out[i], sizeof(unsigned char), width_Scale, pFile);
	}

	//���� �޸� ����
	for (int i = 0; i < height_Scale; i++)
	{
		delete[] Image_Out[i];
	}
	delete[] Image_Out;

	fclose(pFile);
}


