#include "hw1.h"

using namespace std;

void main()
{
	HW hw;
	hw.ImageOpen();
	//cout << "file open" << endl;
	hw.ImageScaling();
	//cout << "savestart"<<endl;
	hw.ImageSave();
	//cout << "save finish" << endl;
}
