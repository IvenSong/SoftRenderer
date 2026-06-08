#include "gpu.h"

GPU* GPU::mInstance = nullptr;
GPU* GPU::getInstance() {
	if (!mInstance) {
		mInstance = new GPU();
	}

	return mInstance;
}

GPU::GPU() {}

GPU::~GPU() {
	if (mFrameBuffer) {
		delete mFrameBuffer;
	}
}

void GPU::initSurface(const uint32_t& width, const uint32_t& height, void* buffer) {
	if (mFrameBuffer) {
		delete mFrameBuffer;
	}

	mFrameBuffer = new FrameBuffer(width, height, buffer);
}

void GPU::clear() {
	if (!mFrameBuffer) {
		return;
	}

	mFrameBuffer->clear();
}

void GPU::drawPoint(int x, int y, const RGBA& color) {
	if (!mFrameBuffer) {
		return;
	}

	mFrameBuffer->setPoint(x, y, color);
}

void GPU::drawPoint(pixel& p) {
	drawPoint(p.x, p.y, p.color);
}

void GPU::drawLine(int x1, int y1, int x2, int y2, const RGBA& color1, const RGBA& color2) {
	pixel p1(x1, y1, color1);
	pixel p2(x2, y2, color2);
	
	raster::RasterizeLine(p1, p2);
}

void GPU::drawLine(pixel& p1, pixel& p2) {

	raster::RasterizeLine(p1, p2);
}

void GPU::drawTriangle(pixel& p1, pixel& p2, pixel& p3) {
	raster::RasterizeTriangle(p1, p2, p3);
}


void GPU::drawImage(const image& img, int x, int y) {
	if (x < 0 || y < 0) {
		return;
	}
	for (int i = 0; i < img.mWidth; ++i) {
		for (int j = 0; j < img.mHeight; ++j) {
			drawPoint(x + i, y + j, img.mData[j * img.mWidth + i]);
		}
	}
}



void GPU::drawImageAlpha(const image& img, int x, int y, int alpha) {
	if (x < 0 || y < 0) {
		return;
	}
	for (int i = 0; i < img.mWidth; ++i) {
		for (int j = 0; j < img.mHeight; ++j) {
			RGBA color = img.mData[j * img.mWidth + i];
			color.mA = static_cast<byte>(std::clamp(alpha, 0, 255));
			drawPoint(x + i, y + j, color);
		}
	}
}

void GPU::setTexture(image* img) {
	mImage = img;
}

RGBA GPU::BilinearInterpolation(const vec2f& UV) {
	int width = mImage->mWidth;
	int height = mImage->mHeight;

	vec2f uv = checkWrap(UV);

	float u = std::clamp(uv.x, 0.0f, 1.0f);
	float v = std::clamp(uv.y, 0.0f, 1.0f); // vertically inversed

	float x = u * (width - 1);
	float y = v * (height - 1);

	int x0 = static_cast<int>(std::floor(x));
	int y0 = static_cast<int>(std::floor(y));

	int x1 = min(x0 + 1, width - 1);
	int y1 = min(y0 + 1, height - 1);

	float tx = x - x0;
	float ty = y - y0;

	const RGBA& c00 = mImage->mData[y0 * width + x0];
	const RGBA& c10 = mImage->mData[y0 * width + x1];
	const RGBA& c01 = mImage->mData[y1 * width + x0];
	const RGBA& c11 = mImage->mData[y1 * width + x1];

	return lerp(lerp(c00, c10, tx), lerp(c01, c11, tx), ty);
}

RGBA GPU::lerp(const RGBA& c1, const RGBA& c2, float weight) {
	return c1 * (1.0f - weight) + c2 * weight;
}

void GPU::setTextureWrap(int x) {
	if (x == 0) {
		mWrap = TEXTURE_WRAP_REPEAT;
	}
	else if (x == 1) {
		mWrap = TEXTURE_WRAP_MIRROR;
	}
}

vec2f GPU::checkWrap(const vec2f& UV) {
	vec2f uv = UV;
	if (uv.x < 0 || uv.x > 1) {
		switch (mWrap)
		{
		case TEXTURE_WRAP_REPEAT:
			uv.x = FRACTION(FRACTION(uv.x) + 1);
		case TEXTURE_WRAP_MIRROR:
			uv.x = 1 - FRACTION(FRACTION(uv.x) + 1);
		}
	}
	if (uv.y < 0 || uv.y > 1) {
		switch (mWrap)
		{
		case TEXTURE_WRAP_REPEAT:
			uv.y = FRACTION(FRACTION(uv.y) + 1);
		case TEXTURE_WRAP_MIRROR:
			uv.y = 1 - FRACTION(FRACTION(uv.y) + 1);
		}
	}
	return uv;
}
