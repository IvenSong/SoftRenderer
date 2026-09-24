#include "clipper.h"

void Clipper::doClipSpace(const int& drawMode, const std::vector<VsOutput>& primitives, std::vector<VsOutput>& outputs)
{
	if (drawMode == DRAW_TRIANGLES) {
		std::vector<VsOutput> primitive;
		std::vector<VsOutput> result;

		for (int i = 0; i < primitives.size(); i += 3) {
			primitive.clear();
			result.clear();
			auto start = primitives.begin() + i;
			auto end = primitives.begin() + i + 3;

			primitive.assign(start, end);

			sutherlandHodgman(drawMode, primitive, result);

			if (result.empty()) {
				continue;
			}

			for (int j = i; j < result.size() - 2; ++j) {
				outputs.push_back(result[0]);
				outputs.push_back(result[j]);
				outputs.push_back(result[j + 1]);
			}
		}

	}
	else if (drawMode == DRAW_LINES) {
		std::vector<VsOutput> primitive;
		std::vector<VsOutput> result;

		for (int i = 0; i < primitives.size(); i += 2) {
			primitive.clear();
			result.clear();
			auto start = primitives.begin() + i;
			auto end = primitives.begin() + i + 2;
			primitive.assign(start, end);

			sutherlandHodgman(drawMode, primitive, result);

			outputs.push_back(result[0]);
			outputs.push_back(result[1]);
	}
}
