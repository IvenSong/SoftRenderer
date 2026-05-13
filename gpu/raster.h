#pragma once
#include "base.h"
#include "gpu.h"
#include "math.h"

namespace raster {

	void RasterizeLine(pixel& p1, pixel& p2);

	void interpollantLine(pixel& p1, pixel& p2, pixel& target);

	void RasterizeTriangle(pixel& p1, pixel& p2, pixel& p3);

	void interpollantTriangle(pixel& p1, pixel& p2, pixel& p3, pixel& target);
};