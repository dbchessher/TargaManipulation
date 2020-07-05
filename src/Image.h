#ifndef IMAGE_H
#define IMAGE_H
#include <string>
#include <vector>

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

    struct Pixel
    {
        unsigned char blueValue;
        unsigned char greenValue;
        unsigned char redValue;
    };

    string _name;
    int _resolution;

    Header* header;
    vector<Pixel>* pixels;

    //TODO: Add Destructor, Copy Constructor, and Copy Assignment Operator
    Image();
    Image(short width, short height);
    ~Image();
    Image& operator=(const Image& rhs);
    Image(const Image& list);

private:
};
#endif