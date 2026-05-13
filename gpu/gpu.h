#pragma once
#include "base.h"
#include "frameBuffer.h"
#include "raster.h"
#include "../application/application.h"
#include "Mathlib.h"
#include "image.h"

#define sgl GPU::getInstance()

/*
* class GPU：
* simulate GPU function
*/
class GPU {
public:
	static GPU* getInstance();
	GPU();

	~GPU();

	//接受外界传入的bmp对应的内存指针以及窗体的宽/高
	void initSurface(const uint32_t& width, const uint32_t& height, void* buffer = nullptr);

	void clear();

	void drawPoint(int x, int y, const RGBA& color);

	void drawPoint(pixel& p);

	void drawLine(int x1, int y1, int x2, int y2, const RGBA& color1, const RGBA& color2);

	void drawLine(pixel& p1, pixel& p2);

	void drawTriangle(pixel& p1, pixel& p2, pixel& p3);

	void interpollate(pixel& p, pixel& a, pixel& b, pixel& c);

	void drawImage(const image& img, int x, int y);

	void drawImageAlpha(const image& img, int x, int y, int alpha);

	void setTexture(image* img);

private:
	static GPU* mInstance;

	bool EnableBlendMode{ true };

	FrameBuffer* mFrameBuffer{ nullptr };

	image* mImage{ nullptr };

};