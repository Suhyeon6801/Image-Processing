#pragma once
#define _CRT_SECURE_NO_WARNINGS

#include<iostream>
#include<stdlib.h>
#include<string.h>
#include<algorithm>//sort

using namespace std;

enum FilterType
{
	FILTER_AVERAGE,
	FILTER_SMOOTH,
	FILTER_SHARPEN,
	FILTER_MEDIAN
};

class Filter
{
private:
	unsigned char** Pad;
	unsigned char** Image_In;
	//median Image
	unsigned char** Image_In_n5;
	unsigned char** Image_In_n10;
	unsigned char** Image_In_n25;

	unsigned char** Image_Out;
	unsigned char** Image_Out2;//sharpenfilter출력영상

	int Height=256;
	int Width=256;
	int FilterSize;
public:
	Filter();
	~Filter();
	void ImageOpen();
	void SpatialFilter(int filtertype,int nFilterSize);
	unsigned char** Padding(unsigned char** In, int nHeight, int nWidth, int nFilterSize);
	void AverageFilter1();
	void AverageFilter2();
	void SmoothFilter();
	void SharpeningFilter();
	void MedianFilter(unsigned char** In, unsigned char** Out,int size);
};

Filter::Filter()
{

}

Filter::~Filter()
{

}

void Filter::ImageOpen()
{
	FILE* pFile;
	pFile = fopen("lena256.raw", "rb");

	Image_In = new unsigned char*[Width];
	for (int h = 0; h < Height; ++h)
	{
		Image_In[h] = new unsigned char[Height];
	}

	if (pFile == NULL) // 예외처리
	{
		cout << "File not found!\n";
		return;
	}

	for (int h = 0; h < 256; h++)
	{
		fread((void*)Image_In[h], sizeof(unsigned char),Width , pFile);
	}

	fclose(pFile);
}

void Filter::SpatialFilter(int filtertype, int nFilterSize)
{
	int nPadSize = (int)nFilterSize / 2;


	Image_Out = new unsigned char*[Width];
	for (int i = 0; i < Width; ++i)
	{
		Image_Out[i] = new unsigned char[Height];
		memset(Image_Out[i], 0, sizeof(unsigned char)*Height);
	}

	FILE *pFile;
	FILE *pFile_n5;
	FILE *pFile_n10;
	FILE *pFile_n25;

	switch (filtertype)
	{
	case FILTER_AVERAGE:
		if (nFilterSize == 3)
		{
			AverageFilter1();
			pFile = fopen("lena256_average3x3.raw", "wb");

			for (int h = 0; h < Height; h++)
			{
				fwrite((const void*)Image_Out[h], sizeof(unsigned char), Width, pFile);
			}

			//동적 메모리 해제
			for (int h = 0; h < Height; h++)
			{
				delete[] Image_Out[h];
			}
			delete[] Image_Out;

			fclose(pFile);
		}
		else if (nFilterSize == 7)
		{
			AverageFilter2();
			pFile = fopen("lena256_average7x7.raw", "wb");

			for (int h = 0; h < Height; h++)
			{
				fwrite((const void*)Image_Out[h], sizeof(unsigned char), Width, pFile);
			}

			//동적 메모리 해제
			for (int h = 0; h < Height; h++)
			{
				delete[] Image_Out[h];
			}
			delete[] Image_Out;

			fclose(pFile);
		}
		else
		{
			for (int h = nPadSize; h < Height + nPadSize; h++)//1은 npadsize=(int)3%2, 2=npadsize
			{
				for (int w = nPadSize; w < Width + nPadSize; w++)
				{
					int pixelSum = 0;
					double average;
					for (int i = 0; i < nFilterSize; i++)
					{
						for (int j = 0; j < nFilterSize; j++)
						{
							pixelSum += Pad[h - nPadSize + i][w - nPadSize + j];//주변 픽셀 값들의 합
						}
					}
					average = pixelSum / (nFilterSize*nFilterSize);//평균값
					Image_Out[h - nPadSize][w - nPadSize] = average;
				}
			}
		}
		break;
	case FILTER_SMOOTH:
		if (nFilterSize == 3)
		{
			SmoothFilter();
			pFile = fopen("lena256_smooth.raw", "wb");

			for (int h = 0; h < Height; h++)
			{
				fwrite((const void*)Image_Out[h], sizeof(unsigned char), Width, pFile);
			}

			//동적 메모리 해제
			for (int h = 0; h < Height; h++)
			{
				delete[] Image_Out[h];
			}
			delete[] Image_Out;

			fclose(pFile);
		}
		break;
	case FILTER_SHARPEN:
		if (nFilterSize == 3)
		{
			SharpeningFilter();
			pFile = fopen("lena256_sharpen.raw", "wb");

			for (int h = 0; h < Height; h++)
			{
				fwrite((const void*)Image_Out2[h], sizeof(unsigned char), Width, pFile);
			}

			//동적 메모리 해제
			for (int h = 0; h < Height; h++)
			{
				delete[] Image_Out2[h];
			}
			delete[] Image_Out2;

			fclose(pFile);
		}
		break;
	case FILTER_MEDIAN:
		if (nFilterSize > 2)
		{
			pFile_n5 = fopen("lena256_n5.raw", "rb");
			pFile_n10 = fopen("lena256_n10.raw", "rb");
			pFile_n25 = fopen("lena256_n25.raw", "rb");

			Image_In_n5 = new unsigned char*[Width];
			for (int h = 0; h < Height; ++h)
			{
				Image_In_n5[h] = new unsigned char[Height];
			}
			Image_In_n10 = new unsigned char*[Width];
			for (int h = 0; h < Height; ++h)
			{
				Image_In_n10[h] = new unsigned char[Height];
			}

			Image_In_n25 = new unsigned char*[Width];
			for (int h = 0; h < Height; ++h)
			{
				Image_In_n25[h] = new unsigned char[Height];
			}

			for (int h = 0; h < Height; h++)
			{
				fread((void*)Image_In_n5[h], sizeof(unsigned char), Width, pFile_n5);
				fread((void*)Image_In_n10[h], sizeof(unsigned char), Width, pFile_n10);
				fread((void*)Image_In_n25[h], sizeof(unsigned char), Width, pFile_n25);
			}

			//lena256_n5에 MedianFilter적용
			MedianFilter(Image_In_n5, Image_Out, nFilterSize);

			pFile_n5 = fopen("lena256_median_n5.raw", "wb");

			for (int h = 0; h < Height; h++)
			{
				fwrite((const void*)Image_Out[h], sizeof(unsigned char), Width, pFile_n5);
			}

			//lena256_n10에 MedianFilter적용
			MedianFilter(Image_In_n10, Image_Out, nFilterSize);

			pFile_n10 = fopen("lena256_median_n10.raw", "wb");

			for (int h = 0; h < Height; h++)
			{
				fwrite((const void*)Image_Out[h], sizeof(unsigned char), Width, pFile_n10);
			}

			//lena256_n25에 MedianFilter적용
			MedianFilter(Image_In_n25, Image_Out, nFilterSize);

			pFile_n25 = fopen("lena256_median_n25.raw", "wb");

			for (int h = 0; h < Height; h++)
			{
				fwrite((const void*)Image_Out[h], sizeof(unsigned char), Width, pFile_n25);
			}

			for (int h = 0; h < Height; h++)
			{
				delete[] Image_Out[h];
			}
			delete[] Image_Out;

			fclose(pFile_n5);
			fclose(pFile_n10);
			fclose(pFile_n25);

		}
		else
		{
			cout << "can't apply median filter!" << endl;
		}
		break;
	default:
		break;
	}

	
}

void Filter::AverageFilter1()
{
	//출력할 영상에 메모리 할당
	Image_Out = new unsigned char*[Width];
	for (int i = 0; i < Width; ++i)
	{
		Image_Out[i] = new unsigned char[Height];
		memset(Image_Out[i], 0, sizeof(unsigned char)*Height);
	}

	Pad=Padding(Image_In, Height, Width, 3);//원본 이미지 Padding,Filtersize=3

	//원본이미지 Padding한거에서 AverageFilter적용
	for (int h = 1; h < Height+2-1; h++)//1은 npadsize=(int)3%2
	{
		for (int w = 1; w < Width+2-1; w++)
		{
			int pixelSum = 0;
			double average;
			for (int i = 0; i < 3; i++)
			{
				for (int j = 0; j < 3; j++)
				{
					pixelSum+=Pad[h-1+i][w-1+j];//주변 픽셀 값들의 합
				}
			}
			average = pixelSum / 9;////합/(size*size)=평균값	
			Image_Out[h-1][w-1] = average;
		}
	}

}

void Filter::AverageFilter2()
{
	//출력할 영상에 메모리 할당
	Image_Out = new unsigned char*[Width];
	for (int i = 0; i < Width; ++i)
	{
		Image_Out[i] = new unsigned char[Height];
		memset(Image_Out[i], 0, sizeof(unsigned char)*Height);
	}

	Pad = Padding(Image_In, Height, Width, 7);//원본 이미지 Padding,Filtersize=7
	for (int h = 3; h < Height + 6 - 3; h++)//3=Padsize, 6=2*Padsize
	{
		for (int w = 3; w < Width + 6 - 3; w++)
		{
			int pixelSum = 0;
			double average;
			for (int i = 0; i < 7; i++)
			{
				for (int j = 0; j < 7; j++)
				{
					pixelSum += Pad[h - 3 + i][w - 3 + j];//주변 픽셀 값들의 합
				}
			}
			average = pixelSum / 49;//합/(size*size)
			Image_Out[h - 3][w - 3] = average;
		}
	}
}

void Filter::SmoothFilter()
{
	//출력할 영상에 메모리 할당
	Image_Out = new unsigned char*[Width];
	for (int i = 0; i < Width; ++i)
	{
		Image_Out[i] = new unsigned char[Height];
		memset(Image_Out[i], 0, sizeof(unsigned char)*Height);
	}

	Pad = Padding(Image_In, Height, Width, 3);//원본 이미지 Padding,Filtersize=3

	//주어진 SmoothMask
	double Mask[3][3] = { {1,2,1},{2,4,2},{1,2,1} };

	//mask와 곱
	for (int h = 1; h < Height + 2 - 1; h++)
	{
		for (int w = 1; w < Width + 2 - 1; w++)
		{
			double sum=0;
			double average;
			for (int i = 0; i < 3; i++)
			{
				for (int j = 0; j < 3; j++)
				{
					sum+=Pad[h - 1 + i][w-1+j] * Mask[i][j];
				}
			}
			average = sum / 16;
			Image_Out[h - 1][w - 1] = average;
		}
	}
}

void Filter::SharpeningFilter()
{
	AverageFilter1();

	//출력할 영상에 메모리 할당
	Image_Out2 = new unsigned char*[Width];
	for (int i = 0; i < Width; ++i)
	{
		Image_Out2[i] = new unsigned char[Height];
		memset(Image_Out2[i], 0, sizeof(unsigned char)*Height);
	}

	Pad = Padding(Image_Out, Height, Width, 3);//average3x3 이미지 Padding,Filtersize=3

	double Mask[3][3] = { {0,-1,0},{-1,5,-1},{0,-1,0} };//주어진 SharpeningMask

	for (int h = 1; h < Height + 2 - 1; h++)
	{
		for (int w = 1; w < Width + 2 - 1; w++)
		{
			int sum = 0;
			for (int i = 0; i < 3; i++)
			{
				for (int j = 0; j < 3; j++)
				{
					sum += Pad[h - 1 + i][w - 1 + j] * Mask[i][j];
				}
			}
			if (sum > 255)
			{
				sum = 255;
				Image_Out2[h - 1][w - 1] = sum;
			}
			else if(sum<0)
			{
				sum = 0;
				Image_Out2[h - 1][w - 1] = sum;
			}
			else
			{
				Image_Out2[h - 1][w - 1] = sum;
			}
		}
	}
}

void Filter::MedianFilter(unsigned char** In, unsigned char** Out, int size)
{
	//Mask를 담을 배열선언 및 할당
	unsigned char** MaskArr = new unsigned char*[size];
	for (int i = 0; i < size; ++i)
	{
		MaskArr[i] = new unsigned char[size];
	}

	unsigned char* arr = new unsigned char[size*size];

	//Median Filter
	Pad = Padding(In, Height, Width, size);

	int p_size = (int)size / 2;
	
	for (int h = p_size; h < Height + p_size; h++)
	{
		for (int w = p_size; w < Width + p_size; w++)
		{
			int median = (int)size*size / 2;
		
			for (int i = 0; i < size; i++)
			{
				for (int j = 0; j < size; j++)
				{
					MaskArr[i][j] = Pad[h - p_size + i][w - p_size + j];
					arr[i*size+j] = MaskArr[i][j];
				}
			}
			//배열 정렬 후 영상출력
			sort(arr, arr + size*size);
			Image_Out[h - p_size][w - p_size] = arr[median];
		}
	}
}

unsigned char** Filter::Padding(unsigned char** In, int nHeight, int nWidth, int nFilterSize)
{
	//적용하는 filtersize에 맞게 boundary넓히기
	int nPadSize = (int)(nFilterSize / 2);
	
	//Pad 할당 
	Pad = new unsigned char*[nWidth+2*nPadSize];
	for (int i = 0; i < nWidth+2*nPadSize; ++i)
	{
		Pad[i] = new unsigned char[nHeight+2*nPadSize];
		memset(Pad[i], 0, sizeof(unsigned char)*(nHeight+2*nPadSize));
	}

	for (int h = 0; h < nHeight; h++)
	{
		for (int w = 0; w < nWidth; w++) {
			Pad[h + nPadSize][w + nPadSize] = In[h][w];
		}
	}

	for (int h = 0; h < nPadSize; h++) {
		for (int w = 0; w < nWidth; w++) {
			Pad[h][w + nPadSize] = In[0][w];
			Pad[h + (nHeight - 1)][w + nPadSize] = In[nHeight - 1][w];
		}
	}

	for (int h = 0; h < nHeight; h++) {
		for (int w = 0; w < nPadSize; w++) {
			Pad[h + nPadSize][w] = In[h][0];
			Pad[h + nPadSize][w + (nWidth - 1)] = In[h][nWidth - 1];
		}
	}

	for (int h = 0; h < nPadSize; h++) {
		for (int w = 0; w < nPadSize; w++) {
			Pad[h][w] = In[0][0];
			Pad[h + (nHeight - 1)][w] = In[nHeight - 1][0];
			Pad[h][w + (nWidth - 1)] = In[0][nWidth - 1];
			Pad[h + (nHeight - 1)][w + (nWidth - 1)] = In[nHeight - 1][nWidth - 1];
		}
	}

	return Pad;
}