#pragma once
#include "assert.h"

namespace Math {

	template <class E>
	class vector2 {
	public:
		E x;
		E y;

		vector2() : x(0), y(0) {};
		vector2(E x, E y) : x(x), y(y) {};

		vector2<E> operator+(const vector2<E>& v) const {
			return vector2(x + v.x, y + v.y);
		}

		vector2<E>& operator+=(const vector2<E>& v) {
			x += v.x;
			y += v.y;
			return *this;
		}

		vector2<E> operator-(const vector2<E>& v) const {
			return vector2(x - v.x, y - v.y);
		}

		vector2<E>& operator-=(const vector2<E>& v) {
			x -= v.x;
			y -= v.y;
			return *this;
		}

		vector2<E> operator*(E n) const {
			return vector2<E>(x * n, y * n);
		}

		vector2<E>& operator*=(E n) {
			x *= n;
			y *= n;
			return *this;
		}
		vector2<E> operator/(E n) const {
			return vector2<E>(x / n, y / n);
		}

		vector2<E>& operator/=(E n) {
			x /= n;
			y /= n;
			return *this;
		}

		const E& operator[](int i) const {
			assert(i == 1 || i == 0);

			if (i == 0) {
				return x;
			}
			return y;
		}

		E& operator[](int i) {
			assert(i == 1 || i == 0);

			if (i == 0) {
				return x;
			}
			return y;
		}

		vector2<E> operator-() const {
			return vector2<E>(-x, -y);
		}
	};

	template <class E>
	class vector3 {
	public:
		E x;
		E y;
		E z;

		vector3() : x(0), y(0), z(0) {};
		vector3(E x, E y, E z) : x(x), y(y), z(z) {};

		vector3<E> operator+(const vector3<E>& v) const {
			return vector3(x + v.x, y + v.y, z + v.z);
		}

		vector3<E>& operator+=(const vector3<E>& v) {
			x += v.x;
			y += v.y;
			z += v.z;
			return *this;
		}

		vector3<E> operator-(const vector3<E>& v) const {
			return vector3(x - v.x, y - v.y, z - v.z);
		}

		vector3<E>& operator-=(const vector3<E>& v) {
			x -= v.x;
			y -= v.y;
			z -= v.z;
			return *this;
		}

		vector3<E> operator*(E n) const {
			return vector3<E>(x * n, y * n, z * n);
		}

		vector3<E>& operator*=(E n) {
			x *= n;
			y *= n;
			z *= n;
			return *this;
		}
		vector3<E> operator/(E n) const {
			return vector3<E>(x / n, y / n, z / n);
		}

		vector3<E>& operator/=(E n) {
			x /= n;
			y /= n;
			z /= n;
			return *this;
		}

		const E& operator[](int i) const {
			assert(i >= 0 && i <= 2);

			if (i == 0) {
				return x;
			}
			if (i == 1) {
				return y;
			}
			return z;
		}

		E& operator[](int i) {
			assert(i >= 0 && i <= 2);

			if (i == 0) {
				return x;
			}
			if (i == 1) {
				return y;
			}
			return z;
		}

		vector3<E> operator-() const {
			return vector3<E>(-x, -y, -z);
		}
	};

	template <class E>
	class vector4 {
	public:
		E x;
		E y;
		E z;
		E w;
		vector4() : x(0), y(0), z(0), w(0) {};
		vector4(E x, E y, E z, E w) : x(x), y(y), z(z), w(w) {};
		vector4<E> operator+(const vector4<E>& v) const {
			return vector4(x + v.x, y + v.y, z + v.z, w + v.w);
		}
		vector4<E>& operator+=(const vector4<E>& v) {
			x += v.x;
			y += v.y;
			z += v.z;
			w += v.w;
			return *this;
		}
		vector4<E> operator-(const vector4<E>& v) const {
			return vector4(x - v.x, y - v.y, z - v.z, w - v.w);
		}
		vector4<E>& operator-=(const vector4<E>& v) {
			x -= v.x;
			y -= v.y;
			z -= v.z;
			w -= v.w;
			return *this;
		}
		vector4<E> operator*(E n) const {
			return vector4<E>(x * n, y * n, z * n, w * n);
		}
		vector4<E>& operator*=(E n) {
			x *= n;
			y *= n;
			z *= n;
			w *= n;
			return *this;
		}
		vector4<E> operator/(E n) const {
			return vector4<E>(x / n, y / n, z / n, w / n);
		}
		vector4<E>& operator/=(E n) {
			x /= n;
			y /= n;
			z /= n;
			w /= n;
			return *this;
		}
		const E& operator[](int i) const {
			assert(i >= 0 && i <= 3);
			if (i == 0) return x;
			if (i == 1) return y;
			if (i == 2) return z;
			return w;
		}
		E& operator[](int i) {
			assert(i >= 0 && i <= 3);
			if (i == 0) return x;
			if (i == 1) return y;
			if (i == 2) return z;
			return w;
		}
		vector4<E> operator-() const {
			return vector4<E>(-x, -y, -z, -w);
		}
	};

	template <class E>
	vector2<E> operator*(E scalar, const vector2<E>& vector) {
		return vector * scalar;
	}

	template <class E>
	vector3<E> operator*(E scalar, const vector3<E>& vector) {
		return vector * scalar;
	}

	template <class E>
	vector4<E> operator*(E scalar, const vector4<E>& vector) {
		return vector * scalar;
	}
}

using vec2i = Math::vector2<int>;
using vec2f = Math::vector2<float>;
using vec3i = Math::vector3<int>;
using vec3f = Math::vector3<float>;
using vec4i = Math::vector4<int>;
using vec4f = Math::vector4<float>;
