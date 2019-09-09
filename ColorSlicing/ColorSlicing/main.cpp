#include "ColorSlicing.h"

void main()
{
	ColorSlicing Color;
	Color.ImageOpen();
	Color.Face();
	Color.SmoothingFilter();
	Color.ImageSave();
}