#pragma once
#include "base.h"
#include "dataStructure.h"
#include "raster.h"

class Clipper {
public:
	static void doClipSpace(const int& drawMode, const std::vector<VsOutput>& primitives, std::vector<VsOutput>& outputs);
private:

	static void sutherlandHodgman(const int& drawMode, const std::vector<VsOutput>& primitive, std::vector<VsOutput>& outputs);

	static bool inside(const vec4f& point, const vec4f& plane);

	static VsOutput intersect(const VsOutput& last, const VsOutput& current, const vec4f& plane);
};