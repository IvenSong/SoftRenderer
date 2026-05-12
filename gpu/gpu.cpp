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
	int max_x = max(p1.x, max(p2.x, p3.x));
	int max_y = max(p1.y, max(p2.y, p3.y));
	int min_x = min(p1.x, min(p2.x, p3.x));
	int min_y = min(p1.y, min(p2.y, p3.y));

	for (int i = min_x; i <= max_x; ++i) {
		for (int j = min_y; j <= max_y; ++j) {
			pixel p(i, j);
			p.color = interpollate(p, p1, p2, p3);
			auto v1 = Math::connect_pixel<float>(p, p1);
			auto v2 = Math::connect_pixel<float>(p, p2);
			auto v3 = Math::connect_pixel<float>(p, p3);
			auto c1 = Math::cross(v1, v2);
			auto c2 = Math::cross(v2, v3);
			auto c3 = Math::cross(v3, v1);

			if ((c1 >= 0 && c2 >= 0 && c3 >= 0) ||
				(c1 <= 0 && c2 <= 0 && c3 <= 0)) {
				drawPoint(p);
			}
		}
	}
}

RGBA GPU::interpollate(pixel& p, pixel& a, pixel& b, pixel& c) {
	auto ab = Math::connect_pixel<float>(a, b);
	auto ac = Math::connect_pixel<float>(a, c);
	float total_area = Math::cross(ab, ac) / 2;

	auto pa = Math::connect_pixel<float>(p, a);
	auto pb = Math::connect_pixel<float>(p, b);
	auto pc = Math::connect_pixel<float>(p, c);

	float alpha = Math::cross<float>(pb, pc) / 2 / total_area;
	float beta = Math::cross<float>(pc, pa) / 2 / total_area;
	float gamma = Math::cross<float>(pa, pb) / 2 / total_area;

	RGBA result = alpha * a.color + beta * b.color + gamma * c.color;

	return result;
}


void GPU::drawImage(const image& img, int x, int y) {
	if (x < 0 || y < 0) {
		return;
	}
	for (int i = x; i < img.mWidth; ++i) {
		for (int j = y; j < img.mHeight; ++j) {
			drawPoint(i, j, img.mData[j * img.mWidth + i]);
		}
	}
}



void GPU::drawImageAlpha(const image& img, int x, int y, int alpha) {
	if (x < 0 || y < 0) {
		return;
	}
	for (int i = x; i < img.mWidth; ++i) {
		for (int j = y; j < img.mHeight; ++j) {
			img.mData[j * img.mWidth + i].mA = alpha;
			drawPoint(i, j, img.mData[j * img.mWidth + i]);
		}
	}
}