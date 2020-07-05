#include <iostream>
#include <vector>
#include "Image.h"

using namespace std;

Image::Image()
{
	// Default constructor
	//header = new Image::Header;
	_name = "";
	_resolution = 0;
	pixels = new vector<Pixel>;
}

Image::Image(short width, short height) 
{
	//header = new Image::Header;

	header->width = width;
	header->height = height;

	_name = "";
	_resolution = (header->width) * (header->height);
	pixels = new vector<Pixel>;
}

Image::~Image()
{
	delete pixels;
	delete header;
}

Image& Image::operator=(const Image& rhs)
{
	if (this != &rhs) {
		delete header;
		//header = new Header;
		*header = *(rhs.header);

		delete pixels;
		pixels = new vector<Pixel>;
		*pixels = *(rhs.pixels);

		_resolution = rhs._resolution;
		_name = rhs._name;
	}

	return *this;
}

Image::Image(const Image& rhs)
{
	_name = rhs._name;
	_resolution = rhs._resolution;
	
	//header* = new Header;
	header->bitsPerPixel = rhs.header->bitsPerPixel;
	header->colorMapDepth = rhs.header->colorMapDepth;
	header->colorMapLength = rhs.header->colorMapLength;
	header->colorMapOrigin = rhs.header->colorMapOrigin;
	header->colorMapType = rhs.header->colorMapType;
	header->dataTypeCode = rhs.header->dataTypeCode;
	header->height = rhs.header->height;
	header->idLength = rhs.header->idLength;
	header->imageDescriptor = rhs.header->imageDescriptor;
	header->width = rhs.header->width;
	header->xOrigin = rhs.header->xOrigin;
	header->yOrigin -=rhs.header->yOrigin;

	pixels = new vector<Pixel>;

	for (unsigned int i = 0; i < rhs.pixels->size(); i++)
	{
		pixels->push_back(rhs.pixels->at(i));
	}
}