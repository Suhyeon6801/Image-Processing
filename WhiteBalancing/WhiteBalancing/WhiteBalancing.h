#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>

using namespace std;

class White
{
private:
	int Height = 256;
	int Width = 256;
	unsigned char** Image_In;
	unsigned char** Image_Out;
	unsigned char* R, *G, *B;

public:
	White();
	~White();
	void ImageOpen();
	void Balancing();
	void ImageSave();
};
