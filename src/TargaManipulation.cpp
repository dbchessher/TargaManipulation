#include <iostream>
#include <fstream>
#include "Image.h"

using namespace std;

int main()
{
    ifstream file("input\\car.tga", ios_base::binary);
    
    unsigned char blueValue = ' ';
    unsigned char greenValue = ' ';
    unsigned char redValue = ' ';

    char idLength = ' ';
    char colorMapType = ' ';
    char dataTypeCode = ' ';
    short colorMapOrigin = 0;
    short colorMapLength = 0;
    char colorMapDepth = ' ';
    short xOrigin = 0;
    short yOrigin = 0;
    short width = 0;
    short height = 0;
    char bitsPerPixel = ' ';
    char imageDescriptor = ' ';

    if (file.is_open())
    {
        file.read(&idLength, sizeof(idLength));
        file.read(&colorMapType, sizeof(colorMapType));
        file.read(&dataTypeCode, sizeof(dataTypeCode)); //this one (2 '\x2')
        file.read((char*)&colorMapOrigin, sizeof(colorMapOrigin));
        file.read((char*)&colorMapLength, sizeof(colorMapLength));
        file.read(&colorMapDepth, sizeof(colorMapDepth));
        file.read((char*)&xOrigin, sizeof(xOrigin));
        file.read((char*)&yOrigin, sizeof(yOrigin));
        file.read((char*)&width, sizeof(width)); //this one
        file.read((char*)&height, sizeof(height)); //this one
        file.read(&bitsPerPixel, sizeof(bitsPerPixel)); //this one
        file.read(&imageDescriptor, sizeof(imageDescriptor));

        cout << "Finished Header" << endl;

        cout << "Getting first pixel values..." << endl;

        file.read((char*)&blueValue, sizeof(blueValue));
        file.read((char*)&greenValue, sizeof(greenValue));
        file.read((char*)&redValue, sizeof(redValue));

        cout << "Finished first pixel" << endl;

        file.close();
        cout << "Closed file" << endl;
    }
    else cout << "Unable to open file";

    Image* firstImage = new Image(width, height);

    firstImage->header->bitsPerPixel = bitsPerPixel;

    int bpp = firstImage->header->bitsPerPixel;

    cout << bpp << endl;

    return 0;
}