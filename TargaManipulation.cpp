#include <iostream>
#include <fstream>
#include "Image.h"

using namespace std;

int main()
{
    ifstream file("input\\car.tga", ios::in | ios::binary);
    char ch;

    if (file.is_open())
    {
        while (!file.eof()) 
        {
            file.get(ch);
            cout << ch << endl;
        }

        file.close();
    }
    else cout << "Unable to open file";

    return 0;
}