#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include "TargaManipulator.h"
#include "../TargaManipulation/src/Image.h"

using namespace std;

//TargaManipulator::TargaManipulator() {}

Image* ReadIntoImage(Image* testImage, string filepath)
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

        testImage->header = new Image::Header;
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

        file.close();
    }
    else cout << "Unable to open file" << endl;

    return testImage*;
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
        writeFile.close();
    }
    else cout << "Unable to open file" << endl;
}

void FillImageHeader(Image* source, Image* copy)
{
    copy->header = new Image::Header;
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

    if (currentValue < 0)
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

Image* Overlay(Image* topLayer, Image* bottomLayer)
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

        if (((double)bottom_NormalizedBlue * 255.0f) <= 127.5)
        {
            result_FinalizedBlue = (int)((2 * (top_NormalizedBlue * bottom_NormalizedBlue) * 255.0f) + 0.5f);
        }
        else
        {
            result_FinalizedBlue = (int)(((1 - 2 * (1 - top_NormalizedBlue) * (1 - bottom_NormalizedBlue)) * 255.0f) + 0.5f);
        }

        if (((double)bottom_NormalizedBlue * 255.0f) <= 127.5)
        {
            result_FinalizedGreen = (int)((2 * (top_NormalizedGreen * bottom_NormalizedGreen) * 255.0f) + 0.5f);
        }
        else
        {
            result_FinalizedGreen = (int)(((1 - 2 * (1 - top_NormalizedGreen) * (1 - bottom_NormalizedGreen)) * 255.0f) + 0.5f);
        }

        if (((double)bottom_NormalizedBlue * 255.0f) <= 127.5)
        {
            result_FinalizedRed = (int)((2 * (top_NormalizedRed * bottom_NormalizedRed) * 255.0f) + 0.5f);
        }
        else
        {
            result_FinalizedRed = (int)(((1 - 2 * (1 - top_NormalizedRed) * (1 - bottom_NormalizedRed)) * 255.0f) + 0.5f);
        }

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

bool CompareImages(Image* generated, Image* example)
{
    bool isMatch = true;

    int resolution = example->_resolution;

    for (int i = 0; i < resolution; i++)
    {
        Image::Pixel* generatedPix = new Image::Pixel;
        Image::Pixel* examplePix = new Image::Pixel;

        generatedPix->blueValue = generated->pixels->at(i).blueValue;
        generatedPix->greenValue = generated->pixels->at(i).greenValue;
        generatedPix->redValue = generated->pixels->at(i).redValue;

        examplePix->blueValue = example->pixels->at(i).blueValue;
        examplePix->greenValue = example->pixels->at(i).greenValue;
        examplePix->redValue = example->pixels->at(i).redValue;

        if (generatedPix->blueValue != examplePix->blueValue)
        {
            return false;
        }

        if (generatedPix->greenValue != examplePix->greenValue)
        {
            return false;
        }

        if (generatedPix->redValue != examplePix->redValue)
        {
            return false;
        }

        delete generatedPix;
        delete examplePix;
    }

    return isMatch;
}

Image* AddToChannel(Image* resultImage, string channel, int value)
{
    Image* adjustedImage = new Image();
    unsigned int resolution = resultImage->_resolution;

    FillImageHeader(resultImage, adjustedImage);

    int result_FinalizedBlue = 0;
    int result_FinalizedGreen = 0;
    int result_FinalizedRed = 0;

    for (unsigned int i = 0; i < resolution; i++)
    {
        if (channel == "blue")
        {
            result_FinalizedBlue = resultImage->pixels->at(i).blueValue;
            result_FinalizedBlue += value;
            result_FinalizedBlue = CheckMinMax(result_FinalizedBlue);

            result_FinalizedGreen = resultImage->pixels->at(i).greenValue;
            result_FinalizedRed = resultImage->pixels->at(i).redValue;
        }

        if (channel == "green")
        {
            result_FinalizedGreen = resultImage->pixels->at(i).greenValue;
            result_FinalizedGreen += value;
            result_FinalizedGreen = CheckMinMax(result_FinalizedGreen);

            result_FinalizedBlue = resultImage->pixels->at(i).blueValue;
            result_FinalizedRed = resultImage->pixels->at(i).redValue;
        }

        if (channel == "red")
        {
            result_FinalizedRed = resultImage->pixels->at(i).redValue;
            result_FinalizedRed += value;
            result_FinalizedRed = CheckMinMax(result_FinalizedRed);

            result_FinalizedBlue = resultImage->pixels->at(i).blueValue;
            result_FinalizedGreen = resultImage->pixels->at(i).greenValue;
        }

        Image::Pixel* nextPix = new Image::Pixel();

        //Create a pixel from these values
        nextPix->blueValue = result_FinalizedBlue;
        nextPix->greenValue = result_FinalizedGreen;
        nextPix->redValue = result_FinalizedRed;

        adjustedImage->pixels->push_back(*nextPix);

        delete nextPix;
    }

    return adjustedImage;
}

Image* ScaleChannel(Image* resultImage, string channel, int value)
{
    Image* adjustedImage = new Image();
    unsigned int resolution = resultImage->_resolution;

    FillImageHeader(resultImage, adjustedImage);

    int result_FinalizedBlue = 0;
    int result_FinalizedGreen = 0;
    int result_FinalizedRed = 0;

    for (unsigned int i = 0; i < resolution; i++)
    {
        if (channel == "blue")
        {
            result_FinalizedBlue = resultImage->pixels->at(i).blueValue;
            result_FinalizedBlue *= value;
            result_FinalizedBlue = CheckMinMax(result_FinalizedBlue);

            result_FinalizedGreen = resultImage->pixels->at(i).greenValue;
            result_FinalizedRed = resultImage->pixels->at(i).redValue;
        }

        if (channel == "green")
        {
            result_FinalizedGreen = resultImage->pixels->at(i).greenValue;
            result_FinalizedGreen *= value;
            result_FinalizedGreen = CheckMinMax(result_FinalizedGreen);

            result_FinalizedBlue = resultImage->pixels->at(i).blueValue;
            result_FinalizedRed = resultImage->pixels->at(i).redValue;
        }

        if (channel == "red")
        {
            result_FinalizedRed = resultImage->pixels->at(i).redValue;
            result_FinalizedRed *= value;
            result_FinalizedRed = CheckMinMax(result_FinalizedRed);

            result_FinalizedBlue = resultImage->pixels->at(i).blueValue;
            result_FinalizedGreen = resultImage->pixels->at(i).greenValue;
        }

        Image::Pixel* nextPix = new Image::Pixel();

        //Create a pixel from these values
        nextPix->blueValue = result_FinalizedBlue;
        nextPix->greenValue = result_FinalizedGreen;
        nextPix->redValue = result_FinalizedRed;

        adjustedImage->pixels->push_back(*nextPix);

        delete nextPix;
    }

    return adjustedImage;
}


void SpiltChannels(Image* sourceImage)
{
    Image* adjustedImage_Blue = new Image();
    Image* adjustedImage_Green = new Image();
    Image* adjustedImage_Red = new Image();

    unsigned int resolution = sourceImage->_resolution;

    FillImageHeader(sourceImage, adjustedImage_Blue);
    FillImageHeader(sourceImage, adjustedImage_Green);
    FillImageHeader(sourceImage, adjustedImage_Red);

    for (unsigned int i = 0; i < resolution; i++)
    {
        //Create a blue pixel from these values
        Image::Pixel* bluePix = new Image::Pixel();
        bluePix->blueValue = sourceImage->pixels->at(i).blueValue;
        bluePix->greenValue = sourceImage->pixels->at(i).blueValue;
        bluePix->redValue = sourceImage->pixels->at(i).blueValue;

        adjustedImage_Blue->pixels->push_back(*bluePix);

        delete bluePix;

        //Create a Green pixel from these values
        Image::Pixel* greenPix = new Image::Pixel();
        greenPix->greenValue = sourceImage->pixels->at(i).greenValue;
        greenPix->blueValue = sourceImage->pixels->at(i).greenValue;
        greenPix->redValue = sourceImage->pixels->at(i).greenValue;

        adjustedImage_Green->pixels->push_back(*greenPix);

        delete greenPix;

        //Create a Red pixel from these values
        Image::Pixel* redPix = new Image::Pixel();
        redPix->redValue = sourceImage->pixels->at(i).redValue;
        redPix->blueValue = sourceImage->pixels->at(i).redValue;
        redPix->greenValue = sourceImage->pixels->at(i).redValue;

        adjustedImage_Red->pixels->push_back(*redPix);

        delete redPix;
    }

    // Write all three to Output
    WriteIntoImage(adjustedImage_Blue, "output\\part8_b.tga");
    WriteIntoImage(adjustedImage_Green, "output\\part8_g.tga");
    WriteIntoImage(adjustedImage_Red, "output\\part8_r.tga");

    Image* example_part8b = new Image();
    Image* example_part8g = new Image();
    Image* example_part8r = new Image();
    string readPath_example_part8b = "examples\\EXAMPLE_part8_b.tga";
    string readPath_example_part8g = "examples\\EXAMPLE_part8_g.tga";
    string readPath_example_part8r = "examples\\EXAMPLE_part8_r.tga";
    ReadIntoImage(example_part8b, readPath_example_part8b);
    ReadIntoImage(example_part8g, readPath_example_part8g);
    ReadIntoImage(example_part8r, readPath_example_part8r);

    bool isMatch = CompareImages(adjustedImage_Blue, example_part8b);
    int compareCount = 0;

    if (isMatch)
    {
        cout << "Test 8a..........Passed!" << endl;
        compareCount++;
    }
    else
    {
        cout << "Test 8a..........Failed!" << endl;

    }
    isMatch = CompareImages(adjustedImage_Green, example_part8g);

    if (isMatch)
    {
        cout << "Test 8..........Passed!" << endl;
        compareCount++;
    }
    else
    {
        cout << "Test 8b..........Failed!" << endl;

    }
    isMatch = CompareImages(adjustedImage_Red, example_part8r);

    if (isMatch)
    {
        cout << "Test 8c..........Passed!" << endl;
        compareCount++;
    }
    else
    {
        cout << "Test 8c..........Failed!" << endl;

    }
}

Image* MergeLayers(Image* blueChannel, Image* greenChannel, Image* redChannel)
{
    Image* resultImage = new Image();

    FillImageHeader(blueChannel, resultImage);

    unsigned int resolution = blueChannel->_resolution;

    for (unsigned int i = 0; i < resolution; i++)
    {
        Image::Pixel* nextPix = new Image::Pixel();

        nextPix->blueValue = blueChannel->pixels->at(i).blueValue;
        nextPix->greenValue = greenChannel->pixels->at(i).greenValue;
        nextPix->redValue = redChannel->pixels->at(i).redValue;

        resultImage->pixels->push_back(*nextPix);

        delete nextPix;
    }

    return resultImage;
}

Image* Rotate180(Image* targetImage)
{
    Image* resultImage = new Image();

    FillImageHeader(targetImage, resultImage);

    int resolution = (int)targetImage->_resolution;

    for (int i = (resolution - 1); i >= 0; i--)
    {
        Image::Pixel* nextPix = new Image::Pixel();

        nextPix->blueValue = targetImage->pixels->at(i).blueValue;
        nextPix->greenValue = targetImage->pixels->at(i).greenValue;
        nextPix->redValue = targetImage->pixels->at(i).redValue;

        resultImage->pixels->push_back(*nextPix);

        delete nextPix;
    }

    return resultImage;
}

Image* Merge4(Image* bottomLeft, Image* bottomRight, Image* topLeft, Image* topRight)
{
    // TODO: Refactor to reduce repeated code.
    Image* resultImage = new Image();

    FillImageHeader(bottomLeft, resultImage);

    resultImage->header->height = (bottomLeft->header->height * 2);
    resultImage->header->width = (bottomLeft->header->width * 2);
    resultImage->_resolution = (resultImage->header->height * resultImage->header->width);

    int count = 0;

    for (short i = 0; i < bottomLeft->header->height; i++)
    {
        for (int j = count; j < count + (int)bottomLeft->header->width; j++)
        {
            Image::Pixel* nextPix = new Image::Pixel();

            nextPix->blueValue = bottomLeft->pixels->at(j).blueValue;
            nextPix->greenValue = bottomLeft->pixels->at(j).greenValue;
            nextPix->redValue = bottomLeft->pixels->at(j).redValue;

            resultImage->pixels->push_back(*nextPix);

            delete nextPix;
        }

        for (int k = count; k < count + (int)bottomRight->header->width; k++)
        {
            Image::Pixel* nextPix = new Image::Pixel();

            nextPix->blueValue = bottomRight->pixels->at(k).blueValue;
            nextPix->greenValue = bottomRight->pixels->at(k).greenValue;
            nextPix->redValue = bottomRight->pixels->at(k).redValue;

            resultImage->pixels->push_back(*nextPix);

            delete nextPix;
        }

        count += bottomLeft->header->width;
    }

    count = 0;

    for (short i = 0; i < topLeft->header->height; i++)
    {
        for (int j = count; j < count + (int)topLeft->header->width; j++)
        {
            Image::Pixel* nextPix = new Image::Pixel();

            nextPix->blueValue = topLeft->pixels->at(j).blueValue;
            nextPix->greenValue = topLeft->pixels->at(j).greenValue;
            nextPix->redValue = topLeft->pixels->at(j).redValue;

            resultImage->pixels->push_back(*nextPix);

            delete nextPix;
        }

        for (int k = count; k < count + (int)topRight->header->width; k++)
        {
            Image::Pixel* nextPix = new Image::Pixel();

            nextPix->blueValue = topRight->pixels->at(k).blueValue;
            nextPix->greenValue = topRight->pixels->at(k).greenValue;
            nextPix->redValue = topRight->pixels->at(k).redValue;

            resultImage->pixels->push_back(*nextPix);

            delete nextPix;
        }

        count += topLeft->header->width;
    }

    return resultImage;
}