#include "raster.h"
#include "gpu.h"

	template <typename T>
	T raster::lerp(const T& v0, const T& v1, float target) {
		return v0 * (1.0f - target) + v1 * target;
	}

	void raster::rasterize(std::vector<VsOutput>& results, const uint32_t& drawMode, const std::vector<VsOutput>& inputs)
	{
		if (drawMode == DRAW_LINES) {
			for (uint32_t i = 0; i < inputs.size(); i += 2) {
				rasterizeLine(results, inputs[i], inputs[i + 1]);
			}
		}

		if (drawMode == DRAW_TRIANGLES) {
			for (uint32_t i = 0; i < inputs.size(); i += 3) {
				rasterizeTriangle(results, inputs[i], inputs[i + 1], inputs[i + 2]);
			}
		}
	}

	void raster::RasterizeLine(pixel& p1, pixel& p2) {
		int x1 = p1.x;
		int x2 = p2.x;
		int y1 = p1.y;
		int y2 = p2.y;
		int dx = abs(x2 - x1);
		int dy = abs(y2 - y1);

		// determine direction of moving
		int sx = (x1 < x2) ? 1 : -1;
		int sy = (y1 < y2) ? 1 : -1;

		// determine the independent variable
		/* Let k = dy/dx;
		* if abs(k) < 1, the independent variable is x;
		* if abs(k) >=1, the independent variable is y;
		*/
		if (dy < dx) {
			int y = y1;
			int p = 2 * dy - dx;

			for (int x = x1; ; x += sx) {
				pixel px(x, y);
				interpollantLine(p1, p2, px);
				sgl->drawPoint(px);
				if (x == x2) {
					break;
				}
				if (p > 0) {
					p -= 2 * dx;
					y += sy;
				}
				p += 2 * dy;
			}
		}
		else {
			int x = x1;
			int p = 2 * dx - dy;

			for (int y = y1; ; y += sy) {
				pixel px(x, y);
				interpollantLine(p1, p2, px);
				sgl->drawPoint(px);
				if (y == y2) {
					break;
				}
				if (p > 0) {
					p -= 2 * dy;
					x += sx;
				}
				p += 2 * dx;
			}
		}
	}

	void raster::interpollantLine(pixel& p1, pixel& p2, pixel& target) {

		float target_distance_x = target.x - p1.x;
		float total_distance_x = p2.x - p1.x;


		float target_distance_y = target.y - p1.y;
		float total_distance_y = p2.y - p1.y;

		float weight = 1.0f;

		if (std::abs(total_distance_x) > std::abs(total_distance_y)) {
			weight = target_distance_x / total_distance_x;
		}
		else if (std::abs(total_distance_y) > 0) {
			weight = target_distance_y / total_distance_y;
		}

		// for color interpollant
		RGBA result;

		result.mR = static_cast<byte>(p2.color.mR * weight + p1.color.mR * (1.0f - weight) + 0.5f);
		result.mG = static_cast<byte>(p2.color.mG * weight + p1.color.mG * (1.0f - weight) + 0.5f);
		result.mB = static_cast<byte>(p2.color.mB * weight + p1.color.mB * (1.0f - weight) + 0.5f);
		result.mA = static_cast<byte>(p2.color.mA * weight + p1.color.mA * (1.0f - weight) + 0.5f);

		target.color = result;
	}

	void raster::RasterizeTriangle(pixel& p1, pixel& p2, pixel& p3) {
		int max_x = max(p1.x, max(p2.x, p3.x));
		int max_y = max(p1.y, max(p2.y, p3.y));
		int min_x = min(p1.x, min(p2.x, p3.x));
		int min_y = min(p1.y, min(p2.y, p3.y));

		for (int i = min_x; i <= max_x; ++i) {
			for (int j = min_y; j <= max_y; ++j) {
				pixel p(i, j);
				raster::interpollantTriangle(p1, p2, p3, p);
				auto v1 = Math::connect_pixel<float>(p, p1);
				auto v2 = Math::connect_pixel<float>(p, p2);
				auto v3 = Math::connect_pixel<float>(p, p3);
				auto c1 = Math::cross(v1, v2);
				auto c2 = Math::cross(v2, v3);
				auto c3 = Math::cross(v3, v1);

				if ((c1 >= 0 && c2 >= 0 && c3 >= 0) ||
					(c1 <= 0 && c2 <= 0 && c3 <= 0)) {
					RGBA resultColor;
					if (sgl->getTexture()) {
						resultColor = sgl->BilinearInterpolation(p.uv);
					}
					else {
						resultColor = p.color;
					}
					sgl->drawPoint(p.x, p.y, resultColor);
				}
			}
		}
	}

	void raster::interpollantTriangle(pixel& p1, pixel& p2, pixel& p3, pixel& target) {
		auto ab = Math::connect_pixel<float>(p1, p2);
		auto ac = Math::connect_pixel<float>(p1, p3);
		float total_area = Math::cross(ab, ac) / 2;

		auto pa = Math::connect_pixel<float>(target, p1);
		auto pb = Math::connect_pixel<float>(target, p2);
		auto pc = Math::connect_pixel<float>(target, p3);

		float alpha = Math::cross<float>(pb, pc) / 2 / total_area;
		float beta = Math::cross<float>(pc, pa) / 2 / total_area;
		float gamma = Math::cross<float>(pa, pb) / 2 / total_area;


		// interpollant for uv
		target.uv = p1.uv * alpha + p2.uv * beta + p3.uv * gamma;
		// interpollant for color
		target.color = alpha * p1.color + beta * p2.color + gamma * p3.color;
	}

	// ----------------------Rendering Pipeline Version-----------------------

	void raster::rasterizeLine(std::vector<VsOutput>& result, const VsOutput& v0, const VsOutput& v1) {
		int x1 = static_cast<int>(v0.mPosition.x);
		int x2 = static_cast<int>(v1.mPosition.x);
		int y1 = static_cast<int>(v0.mPosition.y);
		int y2 = static_cast<int>(v1.mPosition.y);
		int dx = abs(x2 - x1);
		int dy = abs(y2 - y1);
		int sx = (x1 < x2) ? 1 : -1;
		int sy = (y1 < y2) ? 1 : -1;

		if (dy < dx) {
			int y = y1;
			int p = 2 * dy - dx;
			for (int x = x1; ; x += sx) {
				VsOutput px;
				px.mPosition.x = static_cast<float>(x);
				px.mPosition.y = static_cast<float>(y);
				interpollantLine(v0, v1, px);
				result.push_back(px);
				if (x == x2) {
					break;
				}
				if (p > 0) {
					p -= 2 * dx;
					y += sy;
				}
				p += 2 * dy;
			}
		}
		else {
			int x = x1;
			int p = 2 * dx - dy;
			for (int y = y1; ; y += sy) {
				VsOutput px;
				px.mPosition.x = static_cast<float>(x);
				px.mPosition.y = static_cast<float>(y);
				interpollantLine(v0, v1, px);
				result.push_back(px);
				if (y == y2) {
					break;
				}
				if (p > 0) {
					p -= 2 * dy;
					x += sx;
				}
				p += 2 * dx;
			}
		}
	}

	void raster::rasterizeTriangle(std::vector<VsOutput>& result, const VsOutput& v0, const VsOutput& v1, const VsOutput& v2) {
		int max_x = static_cast<int>(max(v0.mPosition.x, max(v1.mPosition.x, v2.mPosition.x)));
		int max_y = static_cast<int>(max(v0.mPosition.y, max(v1.mPosition.y, v2.mPosition.y)));
		int min_x = static_cast<int>(min(v0.mPosition.x, min(v1.mPosition.x, v2.mPosition.x)));
		int min_y = static_cast<int>(min(v0.mPosition.y, min(v1.mPosition.y, v2.mPosition.y)));

		for (int i = min_x; i <= max_x; ++i) {
			for (int j = min_y; j <= max_y; ++j) {
				VsOutput p;
				p.mPosition.x = static_cast<float>(i);
				p.mPosition.y = static_cast<float>(j);
				interpollantTriangle(v0, v1, v2, p);
				auto a = static_cast<vec2f>(v0.mPosition) - static_cast<vec2f>(p.mPosition);
				auto b = static_cast<vec2f>(v1.mPosition) - static_cast<vec2f>(p.mPosition);
				auto c = static_cast<vec2f>(v2.mPosition) - static_cast<vec2f>(p.mPosition);
				auto c1 = Math::cross(a, b);
				auto c2 = Math::cross(b, c);
				auto c3 = Math::cross(c, a);

				if ((c1 >= 0 && c2 >= 0 && c3 >= 0) ||
					(c1 <= 0 && c2 <= 0 && c3 <= 0)) {
					result.push_back(p);
				}
			}
		}
	}

	void raster::interpollantTriangle(const VsOutput& v0, const VsOutput& v1, const VsOutput& v2, VsOutput& target) {
		auto ab = static_cast<vec2f>(v1.mPosition) - static_cast<vec2f>(v0.mPosition);
		auto ac = static_cast<vec2f>(v2.mPosition) - static_cast<vec2f>(v0.mPosition);
		float total_area = Math::cross(ab, ac) / 2;

		auto pa = static_cast<vec2f>(v0.mPosition) - static_cast<vec2f>(target.mPosition);
		auto pb = static_cast<vec2f>(v1.mPosition) - static_cast<vec2f>(target.mPosition);
		auto pc = static_cast<vec2f>(v2.mPosition) - static_cast<vec2f>(target.mPosition);
		float alpha = Math::cross<float>(pb, pc) / 2 / total_area;
		float beta = Math::cross<float>(pc, pa) / 2 / total_area;
		float gamma = Math::cross<float>(pa, pb) / 2 / total_area;

		target.mOneOverW = Math::lerp(v0.mOneOverW, v1.mOneOverW, v2.mOneOverW, alpha, beta, gamma);

		target.mUV = v0.mUV * alpha + v1.mUV * beta + v2.mUV * gamma;
		target.mColor = alpha * v0.mColor + beta * v1.mColor + gamma * v2.mColor;
		target.mPosition.z = alpha * v0.mPosition.z + beta * v1.mPosition.z + gamma * v2.mPosition.z;
		// target.mPosition.w = alpha * v0.mPosition.w + beta * v1.mPosition.w + gamma * v2.mPosition.w;
	}



	// rendering pipeline version
	void raster::interpollantLine(const VsOutput& v0, const VsOutput& v1, VsOutput& target) {
		float target_distance_x = target.mPosition.x - v0.mPosition.x;
		float total_distance_x = v1.mPosition.x - v0.mPosition.x;

		float target_distance_y = target.mPosition.y - v0.mPosition.y;
		float total_distance_y = v1.mPosition.y - v0.mPosition.y;

		float weight = 1.0f;

		if (std::abs(total_distance_x) > std::abs(total_distance_y)) {
			weight = target_distance_x / total_distance_x;
		}
		else if (std::abs(total_distance_y) > 0.0f) {
			weight = target_distance_y / total_distance_y;
		}

		// target.mPosition = lerp(v0.mPosition, v1.mPosition, weight);
		target.mPosition.z = raster::lerp(v0.mPosition.z, v1.mPosition.z, weight);
		target.mPosition.w = raster::lerp(v0.mPosition.w, v1.mPosition.w, weight);
		target.mColor = raster::lerp(v0.mColor, v1.mColor, weight);
		target.mUV = raster::lerp(v0.mUV, v1.mUV, weight);
}





