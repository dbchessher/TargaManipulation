#ifndef TARGAMANIPULATOR_H
#define TARGAMANIPULATOR_H
#include <string>
#include <vector>
#include "../TargaManipulation/src/Image.h"

using namespace std;

static class TargaManipulator {
public:

    //TargaManipulator();

    static void ReadIntoImage(Image* testImage, string filepath);

    static void WriteIntoImage(Image* writeImage, string filepath);

    static void FillImageHeader(Image* source, Image* copy);

    static int CheckMinMax(int currentValue);

    static Image* Multiply(Image* topLayer, Image* bottomLayer);

    static Image* Subtract(Image* target, Image* source);

    static Image* Screen(Image* topLayer, Image* bottomLayer);

    static Image* Overlay(Image* topLayer, Image* bottomLayer);

    static bool CompareImages(Image* generated, Image* example);

    static Image* AddToChannel(Image* resultImage, string channel, int value);

    static Image* ScaleChannel(Image* resultImage, string channel, int value);

    static void SpiltChannels(Image* sourceImage);

    static Image* MergeLayers(Image* blueChannel, Image* greenChannel, Image* redChannel);

    static Image* Rotate180(Image* targetImage);

    static Image* Merge4(Image* bottomLeft, Image* bottomRight, Image* topLeft, Image* topRight);
};
#endif