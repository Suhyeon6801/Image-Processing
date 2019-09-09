#pragma once
#define _CRT_SECURE_NO_WARNINGS

#include<iostream>
#include<stdlib.h>
#include<string.h>

#define nHeight 256
#define nWidth 256

using namespace std;

class Histogram
{
private:
	unsigned char** Image_In;
	unsigned char** Image_Out;
	int* Hist;
public:
	Histogram();
	~Histogram();
	void ImageOpen();
	void HistogramEqualization();
	void ImageSave();
};

Histogram::Histogram() {}

Histogram::~Histogram(){}

void Histogram::ImageOpen()
{
	FILE* pFile;
	pFile = fopen("input.raw", "rb");

	Image_In = new unsigned char*[nWidth];
	for (int h = 0; h < nHeight; ++h)
	{
		Image_In[h] = new unsigned char[nHeight];
	}

	if (pFile == NULL) // 예외처리
	{
		cout << "File not found!\n";
		return;
	}

	for (int h = 0; h < nHeight; h++)
	{
		fread((void*)Image_In[h], sizeof(unsigned char), nWidth, pFile);
	}
	fclose(pFile);
}

void Histogram::HistogramEqualization()
{
	if (Hist == NULL)
	{
		Hist = new int[256];
	}
	memset(Hist, 0, sizeof(int) * 256);

	//Get Histogram
	for (int h = 0; h < nHeight; h++)
	{
		for (int w = 0; w < nWidth; w++)
		{
			Hist[Image_In[h][w]]++;
		}		
	}

	//Normalize Histogram
	int nMax = 0;
	for (int n = 0; n < 256; n++)
	{
		if (nMax < Hist[n])
		{
			nMax = Hist[n];
		}
	}

	//Histogram Equalization
	double Hist_CDF[256] = { 0,0 };

	//hist(i)/WH
	double hist_sum = 0;
	for (int i = 0; i < 256; i++)
	{
		hist_sum += Hist[i];
		Hist_CDF[i] = hist_sum / (nHeight*nWidth);
	}

	Image_Out = new unsigned char*[nWidth];
	for (int i = 0; i < nWidth; ++i)
	{
		Image_Out[i] = new unsigned char[nHeight];
		memset(Image_Out[i], 0, sizeof(unsigned char)*nHeight);
	}

	//out(j)=round(cdf(j)*256)
	for (int h = 0; h < nHeight; h++)
	{
		for (int w = 0; w < nWidth; w++)
		{
			Image_Out[h][w] = Hist_CDF[Image_In[h][w]] * 255;
		}
	}
}

void Histogram::ImageSave()
{
	FILE *pFile;
	pFile = fopen("new_input.raw", "wb");


	for (int h= 0; h < nHeight; h++)
	{
		fwrite((const void*)Image_Out[h], sizeof(unsigned char), nWidth, pFile);
	}

	//동적 메모리 해제
	for (int h = 0; h < nHeight; h++)
	{
		delete[] Image_Out[h];
	}
	delete[] Image_Out;

	fclose(pFile);
}