#include "image.h"
#define STB_IMAGE_IMPLEMENTATION // a requirement of using stb_image.h
#include "stb_image.h"


image::image(const uint32_t width, const uint32_t height, RGBA* data)
    : mWidth(width), mHeight(height), mData(nullptr) {
    if (data) {
        mData = new RGBA[width * height]; // apply a chunk of memory
        memcpy(mData, data, width * height * sizeof(RGBA)); // paste data to mData (data is a temporary memory, so we must copy it into the real memory space)
    }
}

image::~image() {
    delete[] mData;
}

image* image::createImage(std::string path) {
    int imageType = 0;
    int width = 0;
    int height = 0;

    // int default, stb_image reckon that the top left corner is the origin of this image
    // but we want that the left buttom corner to be the origin, so we use this two convert it
    stbi_set_flip_vertically_on_load(false);

    unsigned char* bits = stbi_load(path.c_str(), &width, &height, &imageType, STBI_rgb_alpha);
    // since color in Windows GDI is GBRA, we need to exchange G and R
    for (int i = 0; i < width * height * 4; i += 4) {
        byte tmp = bits[i];
        bits[i] = bits[i + 2];
        bits[i + 2] = tmp;
    }

    image* img = new image(width, height, (RGBA*)bits);

    stbi_image_free(bits);

    return img;
}

void image::destroyImage(image* img) {
    if (img) {
        delete img;
    }
}