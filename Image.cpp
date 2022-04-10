//
// Created by floodd on 23/03/2022.
//
#include <iostream>
#include <fstream>
#include <iomanip>
#include <cstring>
#include "Image.h"


bool Image::load(string filename) {
    ifstream ifs(filename, std::ios::binary);
    string magicNumber;
    ifs >> magicNumber;
    ifs >> this->w;
    ifs >> this->h;
    int p;
    ifs >> p;
    ifs.ignore(256, '\n');
    if (p == 255) {
        delete[] pixels;
        pixels = new Rgb[w * h];
        unsigned char pix[3];
        for (int i = 0; i < w * h; i++) {
            ifs.read(reinterpret_cast<char *>(pix), 3);
            this->pixels[i].r = pix[0];
            this->pixels[i].g = pix[1];
            this->pixels[i].b = pix[2];
        }
        return true;
    }


    return false;

}

bool Image::loadRaw(string filename) {
    return false;
}

bool Image::savePPM(string filename)
{
    if (this->w == 0 || this->h == 0) { fprintf(stderr, "Can't save an empty image\n"); return false; }
    std::ofstream ofs;
    try {
        ofs.open(filename, std::ios::binary); // need to spec. binary mode for Windows users
        if (ofs.fail())
        {
            throw("Can't open output file");
            return false;
        }
        ofs << "P6\n" << this->w << " " << this->h << "\n255\n";
        unsigned char r, g, b;
        // loop over each pixel in the image, clamp and convert to byte format
        for (int i = 0; i < this->w * this->h; ++i) {
            r = this->pixels[i].r;
            g = this->pixels[i].g;
            b = this->pixels[i].b;
            ofs << r << g << b;
        }
        ofs.close();
    }
    catch (const char *err) {
        fprintf(stderr, "%s\n", err);
        ofs.close();
        return false;
    }
    return true;
}


void Image::filterRed() {
    for (int i = 0; i < w*h; i++) {
        pixels[i].r = ((this->pixels[i].r) + (this->pixels[i].g) + (this->pixels[i].b))/3;
        pixels[i].g =  0;
        pixels[i].b = 0;
    }

}

void Image::filterGreen() {

}

void Image::filterBlue() {

}

void Image::greyScale() {
    for (int i = 0; i < w*h; i++)
    {
        unsigned char greyscaleValue = ((this->pixels[i].r) + (this->pixels[i].g) + (this->pixels[i].b))/3;
        this->pixels[i].r = greyscaleValue;
        this->pixels[i].g = greyscaleValue;
        this->pixels[i].b = greyscaleValue;
    }
}



void Image::flipHorizontal() {

        }

void Image::flipVertically() {


}

void Image::AdditionalFunction2() {

}

void Image::AdditionalFunction3() {

}

void Image::AdditionalFunction1() {

}

/* Functions used by the GUI - DO NOT MODIFY */
int Image::getWidth() {
    return w;
}

int Image::getHeight() {
    return h;
}

Rgb *Image::getImage() {
    return pixels;
}