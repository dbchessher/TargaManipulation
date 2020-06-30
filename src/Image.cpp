#include <iostream>
#include <vector>
using namespace std;

#include "Image.h"

Image::Image()
{
	// Default constructor
	header = new Header;
	_name = "";
	_resolution = 0;
	pixels = new vector<Pixel>;
}

Image::Image(short width, short height) 
{
	header = new Header;

	header->width = width;
	header->height = height;

	_name = "";
	_resolution = (header->width) * (header->height);
	pixels = new vector<Pixel>;
}
