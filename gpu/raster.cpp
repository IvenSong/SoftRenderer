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
}