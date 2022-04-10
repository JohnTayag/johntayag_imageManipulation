//
// Created by floodd on 22/03/2022.
//

// Created by floodd on 11/02/2022.
//


#ifndef SDLTEST_TGA_FILEHANDLER_H
#define SDLTEST_TGA_FILEHANDLER_H
#include <iostream>
#include <fstream>
#include <iomanip>
using namespace std;
struct Rgb
{
    Rgb() : r(1), g(1), b(1)  {}
    Rgb(unsigned char rr) : r(rr), g(rr), b(rr) {}
    Rgb(unsigned char rr, unsigned char gg, unsigned char bb) : r(rr), g(gg), b(bb) {}
    unsigned char r, g, b;
};

class Image {



    Image(const unsigned int _w, const unsigned int _h) :
            w(_w), h(_h), pixels(nullptr)
    {
        pixels = new Rgb[w * h];

    }

    unsigned int w, h;
    Rgb *pixels;
public:
    Image() : w(300), h(400)
    {
        pixels = new Rgb[w * h];
    }
    bool savePPM(string filename);
    bool load(string filename);
    bool loadRaw(string filename);
    void filterRed();
    void filterGreen();
    void filterBlue();
    void greyScale();
    void flipHorizontal();
    void flipVertically();
    void AdditionalFunction1();
    void AdditionalFunction2();
    void AdditionalFunction3();

    /*Functions used in the GUI - DO NOT MODIFY */
    Rgb* getImage();
    int getWidth();
    int getHeight();
    ~Image() { if (pixels != nullptr) delete [] pixels; }

};



#endif //SDLTEST_TGA_FILEHANDLER_H

