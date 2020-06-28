#ifndef IMAGE_H
#define IMAGE_H
#include <string>
//#include <vector>
using namespace std;

class Image {
public:

    struct Header
    {
        char idLength;
        char colorMapType;
        char dataTypeCode;
        short colorMapOrigin;
        short colorMapLength;
        char colorMapDepth;
        short xOrigin;
        short yOrigin;
        short width;
        short height;
        char bitsPerPixel;
        char imageDescriptor;
    };

    Image();
    Image(short width, short height);

    //TODO: Add Pixel class and create a data structure to 
    //store all Pixels in an image based on image resoultion

private:
    string _name;
    int _resolution;
};

#endif