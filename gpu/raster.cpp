#include "raster.h"

namespace raster {

	// void raster::RasterizeLine(pixel p) {

	//}

	void raster::RasterizeLine(pixel& p1, pixel& p2, const RGBA& color) {
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
			int p = dx - 2 * dy;

			for (int x = x1; x != x2; x += sx) {
				sgl->drawPoint(x, y, color);
				if (p > 0) {
					p += 2 * dx;
					y += sy;
				}
				p += 2 * dy;
			}
		}
		else {
			int x = x1;
			int p = dy - 2 * dx;

			for (int y = y1; y != y2; y += sy) {
				sgl->drawPoint(x, y, color);
				if (p > 0) {
					p += 2 * dy;
					x += sx;
				}
				p += 2 * dx;
			}
		}
	}
}