#include"WhiteBalancing.h"

White::White() {}
White::~White() {}
void White::ImageOpen()
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

void White::Balancing()
{
	R = new unsigned char[256 * 256];
	G = new unsigned char[256 * 256];
	B = new unsigned char[256 * 256];

	unsigned char* In = new unsigned char[Height*Width * 3];
	unsigned char* Out = new unsigned char[Height*Width * 3];

	for (int h = 0; h < Height; h++)
	{
		for (int w = 0; w < Width * 3; w++)
		{
			In[h * 256*3 + w] = Image_In[h][w];
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
			if (R[i * 256 + j]>= 182 && R[i * 256 + j] <= 223)
			{
				R[i * 256 + j] = 255;
			}
			else
			{
				R[i * 256 + j] += 50;
			}
			if (G[i * 256 + j] >= 155 && G[i * 256 + j] <= 198)
			{
				G[i * 256 + j] = 255;
			}
			else
			{
				G[i * 256 + j] += 50;
			}
			if (B[i * 256 + j] >= 100 && B[i * 256 + j] <= 155)
			{
				B[i * 256 + j] = 255;
			}
			else
			{
				B[i * 256 + j] += 50;
			}
		}
	}

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
		Image_Out[i] = new unsigned char[Width*3];
		memset(Image_Out[i], 0, sizeof(unsigned char)*Width*3);
	}

	for (int h = 0; h < Height; h++)
	{
		for (int w = 0; w < Width * 3; w++)
		{
			Image_Out[h][w] = Out[h * 256*3 + w];
		}
	}
}

void White::ImageSave()
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