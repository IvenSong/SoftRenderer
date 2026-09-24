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

			for (size_t j = 1; j + 1 < result.size(); ++j) {
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
}

void Clipper::sutherlandHodgman(const int& drawMode, const std::vector<VsOutput>&primitive, std::vector<VsOutput>&outputs) {
	
	std::vector<vec4f> clipPlanes = {
	vec4f(0.0f,  0.0f,  0.0f, 1.0f),  // w£ºw >= 0
	vec4f(0.0f,  0.0f,  1.0f, 1.0f), // near z + w >= 0
	vec4f(0.0f,  0.0f, -1.0f, 1.0f), // far w - z >= 0
	vec4f(1.0f,  0.0f,  0.0f, 1.0f), // left x + w >= 0
	vec4f(-1.0f,  0.0f,  0.0f, 1.0f), // right w - x >= 0
	vec4f(0.0f,  1.0f,  0.0f, 1.0f), // down y + w >= 0
	vec4f(0.0f, -1.0f,  0.0f, 1.0f) // up w - y >= 0
	};
	
	outputs = primitive;
	std::vector<VsOutput> inputs;

	for (int i = 0; i < clipPlanes.size(); ++i) { // check every clip plane
		inputs = outputs;
		outputs.clear();

		for (int p = 0; p < inputs.size(); ++p) {
			// get point p
			auto current = inputs[p];
			// get last point s
			auto last = inputs[(p + inputs.size() - 1) % inputs.size()];

			// P is inside
			if (inside(current.mPosition, clipPlanes[i])) {
				if (drawMode != DRAW_LINES || p != inputs.size() - 1) {
					if (!inside(last.mPosition, clipPlanes[i])) {
						auto intersectPoint = intersect(last, current, clipPlanes[i]);

						outputs.push_back(intersectPoint);
					}
				}

				// in case 3/4, we must out put P
				outputs.push_back(current);
			}

			// P is not inside
			else {
				if (drawMode != DRAW_LINES || p != inputs.size() - 1) {
					if (inside(last.mPosition, clipPlanes[i])) {
						auto intersectPoint = intersect(last, current, clipPlanes[i]);
						outputs.push_back(intersectPoint);
					}
				}
				// if S is not inside, output nothing
			}
		}

		


	}

}

bool Clipper::inside(const vec4f& point, const vec4f& plane)
{
	return Math::dot(point, plane) >= 0.0f;
}

VsOutput Clipper::intersect(const VsOutput& last, const VsOutput& current, const vec4f& plane)
{
	VsOutput output;

	float distanceLast = Math::dot(last.mPosition, plane);
	float distanceCurrent = Math::dot(current.mPosition, plane);
	float weight = distanceLast / (distanceLast - distanceCurrent);

	output.mPosition = Math::lerp(last.mPosition, current.mPosition, weight);
	output.mColor = Math::lerp(last.mColor, current.mColor, weight);
	output.mUV = Math::lerp(last.mUV, current.mUV, weight);


	return output;
}
