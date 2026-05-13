#pragma once
#include "base.h"


namespace Math {
	template <typename E>
	E cross(const vector2<E>& v1, const vector2<E>& v2) {
		return v1.x * v2.y - v1.y * v2.x;
	}

	template <typename E>
	vector3<E> cross(const vector3<E>& v1, const vector3<E>& v2) {
		return vector3(v1.y * v2.z - v2.y * v1.z, v2.x * v1.z - v1.x * v2.z, v1.x * v2.y - v2.x * v1.y);
	}


	template <typename E>
	E dot(const vector2<E>& v1, const vector2<E>& v2) {
		E count = 0;
		for (int i = 0; i < 2; ++i) {
			count += (v1[i] * v2[i]);
		}
		return count;
	}

	template <typename E>
	E dot(const vector3<E>& v1, const vector3<E>& v2) {
		E count = 0;
		for (int i = 0; i < 3; ++i) {
			count += (v1[i] * v2[i]);
		}
		return count;
	}

	template <typename E>
	E dot(const vector4<E>& v1, const vector4<E>& v2) {
		E count = 0;
		for (int i = 0; i < 4; ++i) {
			count += (v1[i] * v2[i]);
		}
		return count;
	}

	template <typename E>
	E length(const vector2<E>& v) {
		E count = 0;
		for (int i = 0; i < 2; ++i) {
			count += pow(v[i], 2);
		}
		return pow(count, 0.5);
	}

	template <typename E>
	E length(const vector3<E>& v) {
		E count = 0;
		for (int i = 0; i < 3; ++i) {
			count += pow(v[i], 2);
		}
		return pow(count, 0.5);
	}

	template <typename E>
	E length(const vector4<E>& v) {
		E count = 0;
		for (int i = 0; i < 4; ++i) {
			count += pow(v[i], 2);
		}
		return pow(count, 0.5);
	}

	template <typename E>
	vector2<E> normalize(const vector2<E>& v) {
		return v / length(v)
	}

	template <typename E>
	vector3<E> normalize(const vector3<E>& v) {
		return v / length(v)
	}

	template <typename E>
	vector4<E> normalize(const vector4<E>& v) {
		return v / length(v)
	}

	template <typename E>
	E lengthSQ(const vector2<E>& v) {
		E count = 0;
		for (int i = 0; i < 2; ++i) {
			count += pow(v[i], 2);
		}
		return count;
	}

	template <typename E>
	E lengthSQ(const vector3<E>& v) {
		E count = 0;
		for (int i = 0; i < 3; ++i) {
			count += pow(v[i], 2);
		}
		return count;
	}

	template <typename E>
	E lengthSQ(const vector4<E>& v) {
		E count = 0;
		for (int i = 0; i < 4; ++i) {
			count += pow(v[i], 2);
		}
		return count;
	}

	template <typename E>
	vector2<E> connect_pixel(const pixel& p1, const pixel& p2) {
		return vector2<E>(p2.x - p1.x, p2.y - p1.y);
	}
}