#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>

using namespace std;

class ColorSlicing
{
private:
	int Height = 256;
	int Width = 256;
	unsigned char** Image_In;
	unsigned char** Image_Out, **Rest_Out;
	unsigned char* Out;
	unsigned char** FaceOut;
	unsigned char* R, *G, *B;
	unsigned char** Pad;
	unsigned char** Face_R, **Face_G, **Face_B;
	unsigned char** R_Out, **G_Out, **B_Out;
	unsigned char** Pad_R, **Pad_G, **Pad_B;

public:
	ColorSlicing();
	~ColorSlicing();
	void ImageOpen();
	void ImageSave();
	void Face();//�󱼿�������
	void Rest();//�󱼾ƴѰ� ����
	void SmoothingFilter();//�󱼿��� smoothing����
	unsigned char** Padding(unsigned char** In, int nHeight, int nWidth, int nFilterSize);
};