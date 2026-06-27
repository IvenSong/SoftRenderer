#pragma once
#include "base.h"
#include "dataStructure.h"
#include "Mathlib.h"

namespace raster {

	void RasterizeLine(pixel& p1, pixel& p2);

	void interpollantLine(pixel& p1, pixel& p2, pixel& target);

	void RasterizeTriangle(pixel& p1, pixel& p2, pixel& p3);

	void interpollantTriangle(pixel& p1, pixel& p2, pixel& p3, pixel& target);

	// ---------------------Rendering Pipeline Reconstruction------------------
	void rasterizeLine(const VsOutput& v0, const VsOutput& v1);

	void interpollantLine(const VsOutput& v0, const VsOutput& v1, VsOutput& target);

	void rasterizeTriangle(std::vector<VsOutput> result, const VsOutput& v0, const VsOutput& v1, const VsOutput& v2);

	void interpollantTriangle(const VsOutput& v0, const VsOutput& v1, const VsOutput& v2, const VsOutput& target);
	
	template <typename T>
	T lerp(const T& v0, const T& v1, float target);

};
