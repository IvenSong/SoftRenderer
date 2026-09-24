#pragma once
#include "base.h"

/*
* class FrameBuffer：
* store the bmp memory pointer
*/
class FrameBuffer {
public:
	FrameBuffer(uint32_t width, uint32_t height, void* buffer = nullptr);
	~FrameBuffer();
	FrameBuffer(const FrameBuffer&) = delete;//不准拷贝复制
	FrameBuffer& operator=(const FrameBuffer&) = delete;

	//清除画布内容
	void clear(const RGBA& color = RGBA(0, 0, 0, 0));

	//传入像素位置，绘制成某种颜色
	void setPoint(int x, int y, const RGBA& color);

	void setPoint(pixel& p);

	void blendMode(bool blend) {
		mEnableBlendMode = blend;
	}

	uint32_t getWidth() {
		return mWidth;
	}
	uint32_t getHeight() {
		return mHeight;
	}

	RGBA* getColor() {
		return mColorBuffer;
	}
	RGBA*		mColorBuffer{ nullptr };

private:
	uint32_t	mWidth{ 0 };
	uint32_t	mHeight{ 0 };
	bool		mExternBuffer{ false };
	bool		mEnableBlendMode{ true };
};