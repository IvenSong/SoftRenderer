#include "raster.h"

namespace raster {

	// void raster::RasterizeLine(pixel p) {

	//}

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

	void interpollantLine(pixel& p1, pixel& p2, pixel& target) {

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
}