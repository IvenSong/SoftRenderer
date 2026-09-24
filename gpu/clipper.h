#pragma once
#include "base.h"
#include "dataStructure.h"
#include "raster.h"

class Clipper {
public:
	static void doClipSpace(const int& drawMode, const std::vector<VsOutput>& primitives,
		std::vector<VsOutput>& outputs, bool enableCullFace, uint32_t frontFace, uint32_t cullface);
	static bool cullFace(const uint32_t& frontFace, const uint32_t& cullface,
		const VsOutput& v0, const VsOutput& v1, const VsOutput& v2);

private:
	static void sutherlandHodgman(const int& drawMode, const std::vector<VsOutput>& primitive, std::vector<VsOutput>& outputs);

	static bool inside(const vec4f& point, const vec4f& plane);

	static VsOutput intersect(const VsOutput& last, const VsOutput& current, const vec4f& plane);
};
