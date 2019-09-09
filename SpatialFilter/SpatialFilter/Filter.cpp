#include"Filter.h"

void main()
{
	int size = 0;
	Filter filter;
	filter.ImageOpen();

	filter.SpatialFilter(0, 3);
	cout << "apply average3x3 filter" << endl << endl;

	filter.SpatialFilter(0, 7);
	cout << "apply average7x7 filter" << endl << endl;

	filter.SpatialFilter(1, 3);
	cout << "apply smooth filter" << endl << endl;

	filter.SpatialFilter(2, 3);
	cout << "apply sharpening filter" << endl << endl;

	cout << "input filtersize for make median filter:";
	cin >> size;
	filter.SpatialFilter(3, size);
	cout << "apply median filter" << endl;

}