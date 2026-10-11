#pragma once
#include "base.h"


class image {
public:
	uint32_t mWidth{ 0 };
	uint32_t mHeight{ 0 };
	RGBA* mData{ nullptr };

	image(const uint32_t width, const uint32_t height, RGBA* data = nullptr);
	~image();

	static image* createImage(std::string path);
	static void destroyImage(image* image);
	static image* createImageFromMemory(const std::string& path, unsigned char* dataIn, uint32_t widthIn, uint32_t heightIn);
};