#include <algorithm>
#include <cstddef>

#include "frameBuffer.h"

FrameBuffer::FrameBuffer(uint32_t width, uint32_t height, void* buffer) {
	mWidth = width;
	mHeight = height;

	if (!buffer) {
		buffer = new RGBA[static_cast<std::size_t>(width) * height];
		mExternBuffer = false;
	}
	else {
		mExternBuffer = true;
	}

	mColorBuffer = static_cast<RGBA*>(buffer);
}

FrameBuffer::~FrameBuffer() {
	if (!mExternBuffer && mColorBuffer) {
		delete[] mColorBuffer;
	}
}

void FrameBuffer::clear(const RGBA& color) {
	if (!mColorBuffer) {
		return;
	}

	std::size_t pixelSize = static_cast<std::size_t>(mWidth) * mHeight;
	std::fill_n(mColorBuffer, pixelSize, color);
}

void FrameBuffer::setPoint(int x, int y, const RGBA& color) {
	if (!mColorBuffer || x < 0 || y < 0 ||
		x >= static_cast<int>(mWidth) || y >= static_cast<int>(mHeight)) {
		return;
	}

	int mappedY = static_cast<int>(mHeight) - 1 - y;
	std::size_t pixelPos = static_cast<std::size_t>(mappedY) * mWidth + static_cast<std::size_t>(x);
	mColorBuffer[pixelPos] = color;
}

void FrameBuffer::setPoint(pixel& p) {
	setPoint(p.x, p.y, p.color);
}