#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include "Image.h"

using namespace std;

void ReadIntoImage(Image* testImage, string filepath)
{
    ifstream file(filepath, ios_base::binary);

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

        testImage->header->idLength = idLength;
        testImage->header->colorMapType = colorMapType;
        testImage->header->dataTypeCode = dataTypeCode;
        testImage->header->colorMapOrigin = colorMapOrigin;
        testImage->header->colorMapLength = colorMapLength;
        testImage->header->colorMapDepth = colorMapDepth;
        testImage->header->xOrigin = xOrigin;
        testImage->header->yOrigin = yOrigin;
        testImage->header->width = width;
        testImage->header->height = height;
        testImage->header->bitsPerPixel = bitsPerPixel;
        testImage->header->imageDescriptor = imageDescriptor;
        testImage->_resolution = width * height;

        count = testImage->_resolution;

        while (!file.eof())
        {
            Image::Pixel* nextPix = new Image::Pixel();

            //Create a pixel from these values
            file.read((char*)&nextPix->blueValue, sizeof(blueValue));
            file.read((char*)&nextPix->greenValue, sizeof(greenValue));
            file.read((char*)&nextPix->redValue, sizeof(redValue));

            //Add pixel to pixel[]
            testImage->pixels->push_back(*nextPix);

            //Delete pixel
            delete nextPix;
        }
        cout << "Finished Reading Pixels" << endl;

        file.close();
    }
    else cout << "Unable to open file" << endl;
}

void WriteIntoImage(Image* writeImage, string filepath)
{
    ofstream writeFile(filepath, ios_base::binary);

    if (writeFile.is_open())
    {
        writeFile.write(&writeImage->header->idLength, sizeof(writeImage->header->idLength));
        writeFile.write(&writeImage->header->colorMapType, sizeof(writeImage->header->colorMapType));
        writeFile.write(&writeImage->header->dataTypeCode, sizeof(writeImage->header->dataTypeCode));
        writeFile.write((char*)&writeImage->header->colorMapOrigin, sizeof(writeImage->header->colorMapOrigin));
        writeFile.write((char*)&writeImage->header->colorMapLength, sizeof(writeImage->header->colorMapLength));
        writeFile.write(&writeImage->header->colorMapDepth, sizeof(writeImage->header->colorMapDepth));
        writeFile.write((char*)&writeImage->header->xOrigin, sizeof(writeImage->header->xOrigin));
        writeFile.write((char*)&writeImage->header->yOrigin, sizeof(writeImage->header->yOrigin));
        writeFile.write((char*)&writeImage->header->width, sizeof(writeImage->header->width));
        writeFile.write((char*)&writeImage->header->height, sizeof(writeImage->header->height));
        writeFile.write(&writeImage->header->bitsPerPixel, sizeof(writeImage->header->bitsPerPixel));
        writeFile.write(&writeImage->header->imageDescriptor, sizeof(writeImage->header->imageDescriptor));

        for (int j = 0; j < writeImage->_resolution; j++)
        {
            writeFile.write((char*)&writeImage->pixels->at(j).blueValue, sizeof(unsigned char));
            writeFile.write((const char*)&writeImage->pixels->at(j).greenValue, sizeof(unsigned char));
            writeFile.write((const char*)&writeImage->pixels->at(j).redValue, sizeof(unsigned char));
        }
        cout << "Finished Writing Pixels" << endl;
        writeFile.close();
    }
    else cout << "Unable to open file" << endl;
}

void FillImageHeader(Image* source, Image* copy)
{
    copy->header->idLength = source->header->idLength;
    copy->header->colorMapType = source->header->colorMapType;
    copy->header->dataTypeCode = source->header->dataTypeCode;
    copy->header->colorMapOrigin = source->header->colorMapOrigin;
    copy->header->colorMapLength = source->header->colorMapLength;
    copy->header->colorMapDepth = source->header->colorMapDepth;
    copy->header->xOrigin = source->header->xOrigin;
    copy->header->yOrigin = source->header->yOrigin;
    copy->header->width = source->header->width;
    copy->header->height = source->header->height;
    copy->header->bitsPerPixel = source->header->bitsPerPixel;
    copy->header->imageDescriptor = source->header->imageDescriptor;
    copy->_resolution = source->_resolution;
}

int CheckMinMax(int currentValue)
{
    if (currentValue > 255)
    {
        return 255;
    }

    if(currentValue < 0)
    {
        return 0;
    }

    return currentValue;
}

Image* Multiply(Image* topLayer, Image* bottomLayer)
{
    unsigned int resolution = topLayer->_resolution;

    Image* mixedImage = new Image();

    FillImageHeader(topLayer, mixedImage);

    for (unsigned int i = 0; i < resolution; i++)
    {
        float top_NormalizedBlue = 0;
        float top_NormalizedGreen = 0;
        float top_NormalizedRed = 0;

        float bottom_NormalizedBlue = 0;
        float bottom_NormalizedGreen = 0;
        float bottom_NormalizedRed = 0;

        int mixed_FinalizedBlue = 0;
        int mixed_FinalizedGreen = 0;
        int mixed_FinalizedRed = 0;

        top_NormalizedBlue = (topLayer->pixels->at(i).blueValue / 255.0f);
        top_NormalizedGreen = (topLayer->pixels->at(i).greenValue / 255.0f);
        top_NormalizedRed = (topLayer->pixels->at(i).redValue / 255.0f);

        bottom_NormalizedBlue = (bottomLayer->pixels->at(i).blueValue / 255.0f);
        bottom_NormalizedGreen = (bottomLayer->pixels->at(i).greenValue / 255.0f);
        bottom_NormalizedRed = (bottomLayer->pixels->at(i).redValue / 255.0f);

        mixed_FinalizedBlue = (int)(((top_NormalizedBlue * bottom_NormalizedBlue) * 255.0f) + 0.5f);
        mixed_FinalizedGreen = (int)(((top_NormalizedGreen * bottom_NormalizedGreen) * 255.0f) + 0.5f);
        mixed_FinalizedRed = (int)(((top_NormalizedRed * bottom_NormalizedRed) * 255.0f) + 0.5f);

        Image::Pixel* nextPix = new Image::Pixel();

        //Create a pixel from these values
        nextPix->blueValue = mixed_FinalizedBlue;
        nextPix->greenValue = mixed_FinalizedGreen;
        nextPix->redValue = mixed_FinalizedRed;

        mixedImage->pixels->push_back(*nextPix);

        delete nextPix;
    }

    return mixedImage;
}

Image* Subtract(Image* target, Image* source)
{
    unsigned int resolution = target->_resolution;

    Image* resultImage = new Image();

    FillImageHeader(target, resultImage);

    for (unsigned int i = 0; i < resolution; i++)
    {
        int target_Blue = 0;
        int target_Green = 0;
        int target_Red = 0;
        
        int source_Blue = 0;
        int source_Green = 0;
        int source_Red = 0;

        int result_Blue = 0;
        int result_Green = 0;
        int result_Red = 0;

        target_Blue = target->pixels->at(i).blueValue;
        target_Green = target->pixels->at(i).greenValue;
        target_Red = target->pixels->at(i).redValue;

        source_Blue = source->pixels->at(i).blueValue;
        source_Green = source->pixels->at(i).greenValue;
        source_Red = source->pixels->at(i).redValue;

        result_Blue = target_Blue - source_Blue;
        result_Green = target_Green - source_Green;
        result_Red = target_Red - source_Red;

        result_Blue = CheckMinMax(result_Blue);
        result_Green = CheckMinMax(result_Green);
        result_Red = CheckMinMax(result_Red);

        Image::Pixel* nextPix = new Image::Pixel();

        //Create a pixel from these values
        nextPix->blueValue = result_Blue;
        nextPix->greenValue = result_Green;
        nextPix->redValue = result_Red;

        resultImage->pixels->push_back(*nextPix);

        delete nextPix;
    }

    return resultImage;
}

Image* Screen(Image* topLayer, Image* bottomLayer)
{
    unsigned int resolution = topLayer->_resolution;

    Image* resultImage = new Image();

    FillImageHeader(topLayer, resultImage);

    for (unsigned int i = 0; i < resolution; i++)
    {
        float top_NormalizedBlue = 0;
        float top_NormalizedGreen = 0;
        float top_NormalizedRed = 0;

        float bottom_NormalizedBlue = 0;
        float bottom_NormalizedGreen = 0;
        float bottom_NormalizedRed = 0;

        int result_FinalizedBlue = 0;
        int result_FinalizedGreen = 0;
        int result_FinalizedRed = 0;

        top_NormalizedBlue = (topLayer->pixels->at(i).blueValue / 255.0f);
        top_NormalizedGreen = (topLayer->pixels->at(i).greenValue / 255.0f);
        top_NormalizedRed = (topLayer->pixels->at(i).redValue / 255.0f);

        bottom_NormalizedBlue = (bottomLayer->pixels->at(i).blueValue / 255.0f);
        bottom_NormalizedGreen = (bottomLayer->pixels->at(i).greenValue / 255.0f);
        bottom_NormalizedRed = (bottomLayer->pixels->at(i).redValue / 255.0f);

        // C = 1 - (1 - A) * (1 - B)
        result_FinalizedBlue = (int)(((1 - (1 - top_NormalizedBlue) * (1 - bottom_NormalizedBlue)) * 255.0f) + 0.5f);
        result_FinalizedGreen = (int)(((1 - (1 - top_NormalizedGreen) * (1 - bottom_NormalizedGreen)) * 255.0f) + 0.5f);
        result_FinalizedRed = (int)(((1 - (1 - top_NormalizedRed) * (1 - bottom_NormalizedRed)) * 255.0f) + 0.5f);

        result_FinalizedBlue = CheckMinMax(result_FinalizedBlue);
        result_FinalizedGreen = CheckMinMax(result_FinalizedGreen);
        result_FinalizedRed = CheckMinMax(result_FinalizedRed);

        Image::Pixel* nextPix = new Image::Pixel();

        //Create a pixel from these values
        nextPix->blueValue = result_FinalizedBlue;
        nextPix->greenValue = result_FinalizedGreen;
        nextPix->redValue = result_FinalizedRed;

        resultImage->pixels->push_back(*nextPix);

        delete nextPix;
    }

    return resultImage;
}

int main()
{
    Image* firstImage = new Image();
    Image* secondImage = new Image();
    Image* thirdImage = new Image();
    Image* fourthImage = new Image();
    Image* fifthImage = new Image();
    Image* sixthImage = new Image();
    Image* seventhImage = new Image();
    Image* eighthImage = new Image();
    Image* ninethImage = new Image();
    Image* tenthImage = new Image();
    Image* eleventhImage = new Image();

    string firstReadPath = "input\\car.tga";
    string secondReadPath = "input\\circles.tga";
    string thirdReadPath = "input\\layer_blue.tga";
    string fourthReadPath = "input\\layer_green.tga";
    string fifthReadPath = "input\\layer_red.tga";
    string sixthReadPath = "input\\layer1.tga";
    string seventhReadPath = "input\\layer2.tga";
    string eighthReadPath = "input\\pattern1.tga";
    string ninethReadPath = "input\\pattern2.tga";
    string tenthReadPath = "input\\text.tga";
    string eleventhReadPath = "input\\text2.tga";

    string firstWritePath = "output\\car.tga";
    string secondWritePath = "output\\circles.tga";
    string thirdWritePath = "output\\layer_blue.tga";
    string fourthWritePath = "output\\layer_green.tga";
    string fifthWritePath = "output\\layer_red.tga";
    string sixthWritePath = "output\\layer1.tga";
    string seventhWritePath = "output\\layer2.tga";
    string eighthWritePath = "output\\pattern1.tga";
    string ninethWritePath = "output\\pattern2.tga";
    string tenthWritePath = "output\\text.tga";
    string eleventhWritePath = "output\\text2.tga";

    ReadIntoImage(firstImage, firstReadPath);
    ReadIntoImage(secondImage, secondReadPath);
    ReadIntoImage(thirdImage, thirdReadPath);
    ReadIntoImage(fourthImage, fourthReadPath);
    ReadIntoImage(fifthImage, fifthReadPath);
    ReadIntoImage(sixthImage, sixthReadPath);
    ReadIntoImage(seventhImage, seventhReadPath);
    ReadIntoImage(eighthImage, eighthReadPath);
    ReadIntoImage(ninethImage, ninethReadPath);
    ReadIntoImage(tenthImage, tenthReadPath);
    ReadIntoImage(eleventhImage, eleventhReadPath);

    // Part 1
    Image* part1 = Multiply(sixthImage, eighthImage);
    WriteIntoImage(part1, "output\\part1.tga");

    // Part 2
    Image* part2 = Subtract(firstImage, seventhImage);
    WriteIntoImage(part2, "output\\part2.tga");

    // Part 3
    Image* part3a = Multiply(sixthImage, ninethImage);
    Image* part3b = Screen(tenthImage, part3a);
    WriteIntoImage(part3b, "output\\part3.tga");

    //WriteIntoImage(firstImage, firstWritePath);
    //WriteIntoImage(secondImage, secondWritePath);
    //WriteIntoImage(thirdImage, thirdWritePath);
    //WriteIntoImage(fourthImage, fourthWritePath);
    //WriteIntoImage(fifthImage, fifthWritePath);
    //WriteIntoImage(sixthImage, sixthWritePath);
    //WriteIntoImage(seventhImage, seventhWritePath);
    //WriteIntoImage(eighthImage, eighthWritePath);
    //WriteIntoImage(ninethImage, ninethWritePath);
    //WriteIntoImage(tenthImage, tenthWritePath);
    //WriteIntoImage(eleventhImage, eleventhWritePath);

    return 0;
}