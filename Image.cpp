#include <iostream>
//#include <vector>
using namespace std;

#include "Image.h"

Image::Image()
{
	// Default constructor
	Header* header = new Header();

	_name = "";
	_resolution = 0;
}

Image::Image(short width, short height) 
{
	Header* header = new Header();

	header->width = width;
	header->height = height;

	_name = "";
	_resolution = width * height;
}
