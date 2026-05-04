#pragma once
#include "base.h"

/*
* class FrameBuffer：
* 存储当前画布对应的bmp的内存指针，作为当前绘图画板
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

private:
	uint32_t	mWidth{ 0 };
	uint32_t	mHeight{ 0 };
	RGBA*		mColorBuffer{ nullptr };
	bool		mExternBuffer{ false };
};