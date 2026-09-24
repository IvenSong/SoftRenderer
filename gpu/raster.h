#pragma once
#include "base.h"
#include "dataStructure.h"
#include "Mathlib.h"

class raster {
public:
	raster();
	~raster();

	static void rasterize(
		std::vector<VsOutput>& results,
		const uint32_t& drawMode,
		const std::vector<VsOutput>& inputs
	);

	static void RasterizeLine(pixel& p1, pixel& p2);

	static void interpollantLine(pixel& p1, pixel& p2, pixel& target);

	static void RasterizeTriangle(pixel& p1, pixel& p2, pixel& p3);

	static void interpollantTriangle(pixel& p1, pixel& p2, pixel& p3, pixel& target);

	// ---------------------Rendering Pipeline Reconstruction------------------
private:
	static void rasterizeLine(
		std::vector<VsOutput>& result,
		const VsOutput& v0,
		const VsOutput& v1
		);

	static void interpollantLine(const VsOutput& v0, const VsOutput& v1, VsOutput& target);

	static void rasterizeTriangle(
		std::vector<VsOutput>& result, 
		const VsOutput& v0, const VsOutput& v1, 
		const VsOutput& v2);

	static void interpollantTriangle(const VsOutput& v0, const VsOutput& v1, const VsOutput& v2, VsOutput& target);
	
	template <typename T>
	static T lerp(const T& v0, const T& v1, float target);

};



#define DRAW_LINES 0
#define DRAW_TRIANGLES 1
