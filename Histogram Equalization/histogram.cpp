#include"histogram.h"

void main()
{
	Histogram histogram;
	histogram.ImageOpen();
	histogram.HistogramEqualization();
	histogram.ImageSave();
	cout << "Apply Histogram Equalization!!" << endl;
}