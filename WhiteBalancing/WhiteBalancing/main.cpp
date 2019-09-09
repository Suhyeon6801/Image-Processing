#include"WhiteBalancing.h"

void main()
{
	White white;
	white.ImageOpen();
	white.Balancing();
	white.ImageSave();
}