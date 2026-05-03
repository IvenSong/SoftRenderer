#pragma once
#include "base.h"
#include "gpu.h"

namespace raster {

	void RasterizeLine(pixel& p1, pixel& p2, const RGBA& color);
};