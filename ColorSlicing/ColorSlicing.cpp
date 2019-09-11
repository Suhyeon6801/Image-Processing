#include"ColorSlicing.h"

ColorSlicing::ColorSlicing(){}
ColorSlicing::~ColorSlicing(){}

void ColorSlicing::ImageOpen()
{
	FILE* pFile;
	pFile = fopen("test.raw", "rb");

	Image_In = new unsigned char*[Height];
	for (int h = 0; h < Width * 3; ++h)
	{
		Image_In[h] = new unsigned char[Width * 3];
	}

	if (pFile == NULL) // 예외처리
	{
		cout << "File not found!\n";
		return;
	}

	for (int h = 0; h < 256; h++)
	{
		fread((void*)Image_In[h], sizeof(unsigned char), Width * 3, pFile);
	}
	fclose(pFile);
}

void ColorSlicing::Rest()
{
	R = new unsigned char[256 * 256];
	G = new unsigned char[256 * 256];
	B = new unsigned char[256 * 256];

	unsigned char* In = new unsigned char[Height*Width * 3];
	unsigned char* Rest = new unsigned char[Height*Width * 3];//얼굴외

	for (int h = 0; h < Height; h++)
	{
		for (int w = 0; w < Width * 3; w++)
		{
			In[h * 256 * 3 + w] = Image_In[h][w];
		}
	}
	for (int i = 0; i < Height; i++)
	{
		for (int j = 0; j < Width; j++)
		{
			R[i * 256 + j] = In[(i * 256 + j) * 3 + 0];
			G[i * 256 + j] = In[(i * 256 + j) * 3 + 1];
			B[i * 256 + j] = In[(i * 256 + j) * 3 + 2];
		}
	}
	for (int i = 0; i < 256; i++)
	{
		for (int j = 0; j < 256; j++)
		{
			if (abs(R[i * 256 + j] - 210) > 70)
			{
				Rest[(i * 256 + j) * 3 + 0] = R[i * 256 + j];
			}
			if (abs(G[i * 256 + j] - 130) > 70)
			{
				Rest[(i * 256 + j) * 3 + 1] = G[i * 256 + j];
			}
			if (abs(B[i * 256 + j] - 110) > 70)
			{
				Rest[(i * 256 + j) * 3 + 2] = B[i * 256 + j];
			}
		}
	}
	for (int i = 0; i < 256; i++)
	{
		for (int j = 0; j < 256; j++)
		{
			Rest[(i * 256 + j) * 3 + 0] = R[i * 256 + j];
			Rest[(i * 256 + j) * 3 + 1] = G[i * 256 + j];
			Rest[(i * 256 + j) * 3 + 2] = B[i * 256 + j];
		}
	}

	Rest_Out = new unsigned char*[Height];
	for (int i = 0; i < Height; ++i)
	{
		Rest_Out[i] = new unsigned char[Width * 3];
		memset(Rest_Out[i], 0, sizeof(unsigned char)*Width * 3);
	}

	for (int h = 0; h < Height; h++)
	{
		for (int w = 0; w < Width * 3; w++)
		{
			Rest_Out[h][w] = Rest[h * 256 * 3 + w];
		}
	}
}

void ColorSlicing::Face()//얼굴추출
{
	R = new unsigned char[256 * 256];
	G = new unsigned char[256 * 256];
	B = new unsigned char[256 * 256];

	unsigned char* In = new unsigned char[Height*Width * 3];
	unsigned char* Face = new unsigned char[Height*Width * 3];//얼굴

	for (int h = 0; h < Height; h++)
	{
		for (int w = 0; w < Width * 3; w++)
		{
			In[h * 256 * 3 + w] = Image_In[h][w];
		}
	}
	for (int i = 0; i < Height; i++)
	{
		for (int j = 0; j < Width; j++)
		{
			R[i * 256 + j] = In[(i * 256 + j) * 3 + 0];
			G[i * 256 + j] = In[(i * 256 + j) * 3 + 1];
			B[i * 256 + j] = In[(i * 256 + j) * 3 + 2];
		}
	}

	for (int i = 0; i < 256; i++)
	{
		for (int j = 0; j < 256; j++)
		{
			if (abs(R[i * 256 + j] - 210) <= 70)
			{
				Face[(i * 256 + j) * 3 + 0] = R[i * 256 + j];
			}
			if (abs(G[i * 256 + j] - 130) <= 70)
			{
				Face[(i * 256 + j) * 3 + 1] = G[i * 256 + j];
			}
			if (abs(B[i * 256 + j] - 110) <= 70)
			{
				Face[(i * 256 + j) * 3 + 2] = B[i * 256 + j];
			}
		}
	}

	for (int i = 0; i < 256; i++)
	{
		for (int j = 0; j < 256; j++)
		{
			Face[(i * 256 + j) * 3 + 0] = R[i * 256 + j];
			Face[(i * 256 + j) * 3 + 1] = G[i * 256 + j];
			Face[(i * 256 + j) * 3 + 2] = B[i * 256 + j];
		}
	}
	
	Face_R = new unsigned char*[Height];
	Face_G = new unsigned char*[Height];
	Face_B = new unsigned char*[Height];

	for (int i = 0; i < Height; ++i)
	{
		Face_R[i] = new unsigned char[Width * 3];
		Face_G[i] = new unsigned char[Width * 3];
		Face_B[i] = new unsigned char[Width * 3];

		memset(Face_R[i], 0, sizeof(unsigned char)*Width * 3);
		memset(Face_G[i], 0, sizeof(unsigned char)*Width * 3);
		memset(Face_B[i], 0, sizeof(unsigned char)*Width * 3);

	}
	for (int h = 0; h < Height; h++)
	{
		for (int w = 0; w < Width * 3; w++)
		{
			Face_R[h][w] = Face[(h * 256 + w) * 3 + 0];
			Face_G[h][w] = Face[(h * 256 + w) * 3 + 1];
			Face_B[h][w] = Face[(h * 256 + w) * 3 + 2];
		}
	}
}

void ColorSlicing::SmoothingFilter()
{
	//출력할 영상에 메모리 할당
	R_Out = new unsigned char*[Height];
	G_Out = new unsigned char*[Height];
	B_Out = new unsigned char*[Height];

	for (int i = 0; i < Height; ++i)
	{
		R_Out[i] = new unsigned char[Width * 3];
		G_Out[i] = new unsigned char[Width * 3];
		B_Out[i] = new unsigned char[Width * 3];

		memset(R_Out[i], 0, sizeof(unsigned char)*Width * 3);
		memset(G_Out[i], 0, sizeof(unsigned char)*Width * 3);
		memset(B_Out[i], 0, sizeof(unsigned char)*Width * 3);
	}

	Pad_R = Padding(Face_R, Height, Width, 3);//원본 이미지 Padding,Filtersize=3
	Pad_G = Padding(Face_G, Height, Width, 3);//원본 이미지 Padding,Filtersize=3
	Pad_B = Padding(Face_B, Height, Width, 3);//원본 이미지 Padding,Filtersize=3


	//주어진 SmoothMask
	double Mask[3][3] = { {1,2,1},{2,4,2},{1,2,1} };

	//mask와 곱
	for (int h = 1; h < Height + 1; h++)
	{
		for (int w = 1; w < Width + 1; w++)
		{
			double sum_R = 0, sum_G = 0, sum_B = 0;
			double average_R, average_G, average_B;
			for (int i = 0; i < 3; i++)
			{
				for (int j = 0; j < 3; j++)
				{
					sum_R += Pad_R[h - 1 + i][w - 1 + j] * Mask[i][j];
					sum_G += Pad_G[h - 1 + i][w - 1 + j] * Mask[i][j];
					sum_B += Pad_B[h - 1 + i][w - 1 + j] * Mask[i][j];
				}
			}
			average_R = sum_R / 16;
			average_G = sum_G / 16;
			average_B = sum_B / 16;

			R_Out[h - 1][w - 1] = average_R;
			G_Out[h - 1][w - 1] = average_G;
			B_Out[h - 1][w - 1] = average_B;
		}
	}

	for (int h = 0; h < Height; h++)
	{
		for (int w = 0; w < Width; w++)
		{
			R[h * 256 + w] = R_Out[h][w];
			G[h * 256 + w] = G_Out[h][w];
			B[h * 256 + w] = B_Out[h][w];
		}
	}
	
	unsigned char* Out = new unsigned char[Height*Width * 3];
	for (int i = 0; i < 256; i++)
	{
		for (int j = 0; j < 256; j++)
		{
			Out[(i * 256 + j) * 3 + 0] = R[i * 256 + j];
			Out[(i * 256 + j) * 3 + 1] = G[i * 256 + j];
			Out[(i * 256 + j) * 3 + 2] = B[i * 256 + j];
		}
	}

	Image_Out = new unsigned char*[Height];
	for (int i = 0; i < Height; ++i)
	{
		Image_Out[i] = new unsigned char[Width * 3];
		memset(Image_Out[i], 0, sizeof(unsigned char)*Width * 3);
	}

	for (int h = 0; h < Height; h++)
	{
		for (int w = 0; w < Width * 3; w++)
		{
			Image_Out[h][w] = Out[h * 256 * 3 + w];
		}
	}
}

unsigned char** ColorSlicing::Padding(unsigned char** In, int nHeight, int nWidth, int nFilterSize)
{
	//적용하는 filtersize에 맞게 boundary넓히기
	int nPadSize = (int)(nFilterSize / 2);

	//Pad 할당 
	Pad = new unsigned char*[nHeight + 2 * nPadSize];
	for (int i = 0; i < nHeight + 2 * nPadSize; ++i)
	{
		Pad[i] = new unsigned char[nWidth + 2 * nPadSize];
		memset(Pad[i], 0, sizeof(unsigned char)*(nWidth + 2 * nPadSize));
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
			Pad[h + (nHeight - 1)][w + (nWidth- 1)] = In[nHeight - 1][nWidth - 1];
		}
	}

	return Pad;
}

void ColorSlicing::ImageSave()
{
	FILE *pFile;
	pFile = fopen("new.raw", "wb");

	for (int h = 0; h < 256; h++)
	{
		fwrite((const void*)Image_Out[h], sizeof(unsigned char), Width * 3, pFile);
	}

	//동적 메모리 해제
	for (int h = 0; h < Height; h++)
	{
		delete[] Image_Out[h];
	}
	delete[] Image_Out;

	fclose(pFile);
}