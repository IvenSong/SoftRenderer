#pragma once
#include "base.h"
#include "gpu.h"
#include "math.h"

namespace raster {

	void RasterizeLine(pixel& p1, pixel& p2);

	void interpollantLine(pixel& p1, pixel& p2, pixel& target);
};