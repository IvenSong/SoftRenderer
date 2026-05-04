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
