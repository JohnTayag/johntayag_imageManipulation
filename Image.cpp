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

bool Image::savePPM(string filename) {
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
    for (int i = 0; i < w*h; i++) {
        pixels[i].r = 0;
        pixels[i].g =  ((this->pixels[i].r) + (this->pixels[i].g) + (this->pixels[i].b))/3;
        pixels[i].b = 0;
    }
}

void Image::filterBlue() {
    for (int i = 0; i < w*h; i++) {
        pixels[i].r = 0;
        pixels[i].g =  0;
        pixels[i].b = ((this->pixels[i].r) + (this->pixels[i].g) + (this->pixels[i].b))/3;
    }
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
    //rgb (r,g,b)
    int pixels[3];

    unsigned int tempPixel;
    unsigned int tempPixel1;
    for (int y = 0; y < h; y++) {

        for (int x = 0; x < w / 2; x++) {
            //5 width
            //(0,0)->5
            //we dont care about the y value
            //thats why we want to just change the x value we store in tempPixel
            tempPixel = x + y * w;
            //1. 5-1-0 = 4, 4+0*5
            tempPixel1 = (w - 1 - x) + y * w;

            //(r,g,b) red value at (0,0) will equal to red value at (4,0) position(because of this->pixels[tempPixel].r)
            //or
            //store r value from r value of (4,0)

            pixels[0] = this->pixels[tempPixel].r;
            pixels[1] = this->pixels[tempPixel].g;
            pixels[2] = this->pixels[tempPixel].b;

            //r value at (4,0) will be (R VALUE FROM (0,0)?)
            this->pixels[tempPixel].r = this->pixels[tempPixel1].r;
            this->pixels[tempPixel].g = this->pixels[tempPixel1].g;
            this->pixels[tempPixel].b = this->pixels[tempPixel1].b;

            //R VALUE AT (0,0) WILL BE (R VALUE FROM line 86-88 we stored? r value of(4,0))
            this->pixels[tempPixel1].r = pixels[0];
            this->pixels[tempPixel1].g = pixels[1];
            this->pixels[tempPixel1].b = pixels[2];
        }
    }
}

void Image::flipVertically() {
    int pixels[3];
    unsigned int tempPixel;
    unsigned int tempPixel1;
    for (int x = 0; x < w; x++) {
        for (int y = 0; y < h / 2; y++) {
            tempPixel = x + y * w;
            tempPixel1 = x + (h - 1 - y) * w;

            pixels[0] = this->pixels[tempPixel].r;
            pixels[1] = this->pixels[tempPixel].g;
            pixels[2] = this->pixels[tempPixel].b;

            this->pixels[tempPixel].r = this->pixels[tempPixel1].r;
            this->pixels[tempPixel].g = this->pixels[tempPixel1].g;
            this->pixels[tempPixel].b = this->pixels[tempPixel1].b;

            this->pixels[tempPixel1].r = pixels[0];
            this->pixels[tempPixel1].g = pixels[1];
            this->pixels[tempPixel1].b = pixels[2];
        }
    }

}

//crop
void Image::AdditionalFunction2(int cx, int cy, int cw, int ch) {
    //place to store new image data
    //channels = how many bytes of memory we want here
    Image *croppedimage = new Image(cw, ch);

    //start looping through size of cropped image
    for (int y = 0; y < ch; y++) {
        //check if your going out of bounds
        if (y + cy > h) { break; }
        for (int x = 0; x < cw; x++) {
            if (x + cx > w) { break; }
            //copy some memory into our cropped
            //copying from data(original image pixels) to cropped image
            std::memcpy(&croppedimage->pixels[(x + y * cw)], &pixels[(x + cx + (y + cy) * w)], 3);
        }
    }

    //readjust the width and height variables because were resizing the image
    w = cw;
    h = ch;
    //delete our old image data
    delete[] this->pixels;
    //set that pointer to croppedimage
    pixels = croppedimage->pixels;
    croppedimage = nullptr;

}

//inverse
void Image::AdditionalFunction3() {

    for (int y = 0; y < h; y++) {
        for (int x = 0; x < w; x++) {
            //loop through each pixel
            //take away the max rgb value from the pixel rgb value
            pixels[y * w + x].r = 255 - pixels[y * w + x].r;
            pixels[y * w + x].g = 255 - pixels[y * w + x].g;
            pixels[y * w + x].b = 255 - pixels[y * w + x].b;

        }
    }
}

//void Image::gammaEncoding(const std::pair &shift) {
//    Image tmp(img.w, img.h);
//    int w = img.w, h = img.h;
//    for (int j = 0; j < h; ++j) {
//        int mod = (j + shift.second) % h;
//        for (int i = 0; i < w; ++i) {
//            int imod = (i + shift.first) % w;
//            tmp[jmod * w + imod] = img[j * w + i];
//        }
//    }
//
//    return tmp;
//}

void Image::AdditionalFunction1() {
    // flip upside down
//int size = h*w;
//
//        for(int i=0;i<size;i++)
//        {
//            int temp = this->pixels[i].r;
//            int temp_one = this->pixels[i].g;
//            int temp_two = this->pixels[i].b;
//            this->pixels[i].r = this->pixels[size].r;
//            this->pixels[i].g = this->pixels[size].g;
//            this->pixels[i].b = this->pixels[size].b;
//            this->pixels[size].r = temp;
//            this->pixels[size].g = temp_one;
//            this->pixels[size].b = temp_two;
//            size--;
//        }


//        for(int c=0;c<w;c++){
//
//            for(int r=h-(h-1);r>-1;r--){
//
//            }
//
//        }
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