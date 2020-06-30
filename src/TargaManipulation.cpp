#include <iostream>
#include <fstream>
#include <vector>
#include "Image.h"
//#include "Pixel.h"

using namespace std;

int main()
{
    Image* firstImage = new Image();
    ifstream file("input\\car.tga", ios_base::binary);
    //ifstream file("input\\circles.tga", ios_base::binary);
    //ifstream file("input\\layer_blue.tga", ios_base::binary);
    //ifstream file("input\\layer_green.tga", ios_base::binary);
    //ifstream file("input\\layer_red.tga", ios_base::binary);
    //ifstream file("input\\layer1.tga", ios_base::binary);
    //ifstream file("input\\layer2.tga", ios_base::binary);
    //ifstream file("input\\pattern1.tga", ios_base::binary);
    //ifstream file("input\\pattern2.tga", ios_base::binary);
    //ifstream file("input\\text.tga", ios_base::binary);
    //ifstream file("input\\text2.tga", ios_base::binary);

    unsigned char colorValue = ' ';

    unsigned char blueValue = ' ';
    unsigned char greenValue = ' ';
    unsigned char redValue = ' ';

    unsigned int count = 0;

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
        file.read(&dataTypeCode, sizeof(dataTypeCode)); //this one
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

        //firstImage = new Image(width, height);
        firstImage->header->dataTypeCode = dataTypeCode;
        firstImage->header->bitsPerPixel = bitsPerPixel;
        firstImage->header->width = width;
        firstImage->header->height = height;
        firstImage->_resolution = width * height;

        count = firstImage->_resolution;

        while (!file.eof())
        {
            Image::Pixel* nextPix = new Image::Pixel();

            //Create a pixel from these values
            file.read((char*)&nextPix->blueValue, sizeof(blueValue));
            file.read((char*)&nextPix->greenValue, sizeof(greenValue));
            file.read((char*)&nextPix->redValue, sizeof(redValue));

            //Add pixel to pixel[]
            firstImage->pixels->push_back(*nextPix);

            //Delete pixel
            delete nextPix;
        }
        cout << "Finished all pixels" << endl;

        file.close();
        cout << "Closed file" << endl;
    }
    else cout << "Unable to open file" << endl;

    ofstream writeFile("output\\car.tga", ios_base::binary);
    //ofstream writeFile("output\\circles.tga", ios_base::binary);
    //ofstream writeFile("output\\layer_blue.tga", ios_base::binary);
    //ofstream writeFile("output\\layer_green.tga", ios_base::binary);
    //ofstream writeFile("output\\layer_red.tga", ios_base::binary);
    //ofstream writeFile("output\\layer1.tga", ios_base::binary);
    //ofstream writeFile("output\\layer2.tga", ios_base::binary);
    //ofstream writeFile("output\\pattern1.tga", ios_base::binary);
    //ofstream writeFile("output\\pattern2.tga", ios_base::binary);
    //ofstream writeFile("output\\text.tga", ios_base::binary);
    //ofstream writeFile("output\\text2.tga", ios_base::binary);
    
    if (writeFile.is_open())
    {
        cout << "Writing Header to file..." << endl;
        writeFile.write(&idLength, sizeof(idLength));
        writeFile.write(&colorMapType, sizeof(colorMapType));

        if (&firstImage->header->dataTypeCode != nullptr)
        {
            writeFile.write(&firstImage->header->dataTypeCode, sizeof(dataTypeCode)); //this one
        }
        else writeFile.write(&dataTypeCode, sizeof(dataTypeCode)); //this one

        writeFile.write((char*)&colorMapOrigin, sizeof(colorMapOrigin));
        writeFile.write((char*)&colorMapLength, sizeof(colorMapLength));
        writeFile.write(&colorMapDepth, sizeof(colorMapDepth));
        writeFile.write((char*)&xOrigin, sizeof(xOrigin));
        writeFile.write((char*)&yOrigin, sizeof(yOrigin));
        
        if (&firstImage->header->width != nullptr)
        {
            writeFile.write((char*)&firstImage->header->width, sizeof(width)); //this one
        }
        else writeFile.write((char*)&width, sizeof(width)); //this one

        if (&firstImage->header->height != nullptr)
        {
            writeFile.write((char*)&firstImage->header->height, sizeof(height)); //this one
        }
        else writeFile.write((char*)&height, sizeof(height)); //this one
        
        writeFile.write(&bitsPerPixel, sizeof(bitsPerPixel)); //this one
        writeFile.write(&imageDescriptor, sizeof(imageDescriptor));
        cout << "Finished Header" << endl;

        cout << "Writing Pixels to file..." << endl;
        for (unsigned int j = 0; j < count; j++) 
        {
            //unsigned char blueColor = firstImage->pixels->at(j).blueValue;
            writeFile.write((char*)&firstImage->pixels->at(j).blueValue, sizeof(unsigned char));
            writeFile.write((const char*)&firstImage->pixels->at(j).greenValue, sizeof(unsigned char));
            writeFile.write((const char*)&firstImage->pixels->at(j).redValue, sizeof(unsigned char));
        }
        cout << "Finished Pixels" << endl;
    }
    else cout << "Unable to open file" << endl;

    writeFile.close();
    return 0;
}