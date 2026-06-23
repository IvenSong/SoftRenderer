#pragma once
#include "base.h"
#include "matrix.h"


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
		return v / length(v);
	}

	template <typename E>
	vector3<E> normalize(const vector3<E>& v) {
		return v / length(v);
	}

	template <typename E>
	vector4<E> normalize(const vector4<E>& v) {
		return v / length(v);
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


	//matrix
/*
* m0 m3 m6
* m1 m4 m7
* m2 m5 m8
*/
	template<typename T>
	Matrix33<T> transpose(const Matrix33<T>& m) {
		Matrix33<T> result;
		auto dst = result.m;
		auto src = m.m;
		dst[0] = src[0]; dst[3] = src[1]; dst[6] = src[2];
		dst[1] = src[3]; dst[4] = src[4]; dst[7] = src[5];
		dst[2] = src[6]; dst[5] = src[7]; dst[8] = src[8];

		return result;
	}

	/*
	* m0 m4 m8	m12
	* m1 m5 m9	m13
	* m2 m6 m10 m14
	* m3 m7 m11	m15
	*/
	template<typename T>
	Matrix44<T> transpose(const Matrix44<T>& m) {
		Matrix44<T> result;
		auto dst = result.m;
		auto src = m.m;

		dst[0] = src[0]; dst[4] = src[1]; dst[8] = src[2]; dst[12] = src[3];
		dst[1] = src[4]; dst[5] = src[5]; dst[9] = src[6]; dst[13] = src[7];
		dst[2] = src[8]; dst[6] = src[9]; dst[10] = src[10]; dst[14] = src[11];
		dst[3] = src[12]; dst[7] = src[13]; dst[11] = src[14]; dst[15] = src[15];

		return result;
	}

	template<typename T>
	Matrix33<T> operator * (const Matrix33<T>& m1, const Matrix33<T>& m2) {
		auto m1Col0 = m1.getColum(0);
		auto m1Col1 = m1.getColum(1);
		auto m1Col2 = m1.getColum(2);

		auto m2Col0 = m2.getColum(0);
		auto m2Col1 = m2.getColum(1);
		auto m2Col2 = m2.getColum(2);

		vector3<T> rCol0, rCol1, rCol2;
		rCol0 = m1Col0 * m2Col0[0] + m1Col1 * m2Col0[1] + m1Col2 * m2Col0[2];
		rCol1 = m1Col0 * m2Col1[0] + m1Col1 * m2Col1[1] + m1Col2 * m2Col1[2];
		rCol2 = m1Col0 * m2Col2[0] + m1Col1 * m2Col2[1] + m1Col2 * m2Col2[2];

		Matrix33<T> result;
		result.setColum(rCol0, 0);
		result.setColum(rCol1, 1);
		result.setColum(rCol2, 2);

		return result;
	}

	template<typename T>
	Matrix44<T> operator * (const Matrix44<T>& m1, const Matrix44<T>& m2) {
		auto m1Col0 = m1.getColum(0);
		auto m1Col1 = m1.getColum(1);
		auto m1Col2 = m1.getColum(2);
		auto m1Col3 = m1.getColum(3);

		auto m2Col0 = m2.getColum(0);
		auto m2Col1 = m2.getColum(1);
		auto m2Col2 = m2.getColum(2);
		auto m2Col3 = m2.getColum(3);

		vector4<T> rCol0, rCol1, rCol2, rCol3;
		rCol0 = m1Col0 * m2Col0[0] + m1Col1 * m2Col0[1] + m1Col2 * m2Col0[2] + m1Col3 * m2Col0[3];
		rCol1 = m1Col0 * m2Col1[0] + m1Col1 * m2Col1[1] + m1Col2 * m2Col1[2] + m1Col3 * m2Col1[3];
		rCol2 = m1Col0 * m2Col2[0] + m1Col1 * m2Col2[1] + m1Col2 * m2Col2[2] + m1Col3 * m2Col2[3];
		rCol3 = m1Col0 * m2Col3[0] + m1Col1 * m2Col3[1] + m1Col2 * m2Col3[2] + m1Col3 * m2Col3[3];

		Matrix44<T> result;
		result.setColum(rCol0, 0);
		result.setColum(rCol1, 1);
		result.setColum(rCol2, 2);
		result.setColum(rCol3, 3);

		return result;
	}

	/*
	* m0 m4 m8	m12
	* m1 m5 m9	m13
	* m2 m6 m10 m14
	* m3 m7 m11	m15
	*/
	template<typename T>
	Matrix44<T> inverse(const Matrix44<T>& src) {
		Matrix44<T> result(static_cast<T>(1));

		T D_22_33 = src.get(2, 2) * src.get(3, 3) - src.get(2, 3) * src.get(3, 2);

		T D_12_23 = src.get(1, 2) * src.get(2, 3) - src.get(1, 3) * src.get(2, 2);
		T D_12_33 = src.get(1, 2) * src.get(3, 3) - src.get(1, 3) * src.get(3, 2);

		T D_21_32 = src.get(2, 1) * src.get(3, 2) - src.get(2, 2) * src.get(3, 1);
		T D_21_33 = src.get(2, 1) * src.get(3, 3) - src.get(2, 3) * src.get(3, 1);

		T D_11_22 = src.get(1, 1) * src.get(2, 2) - src.get(1, 2) * src.get(2, 1);
		T D_11_23 = src.get(1, 1) * src.get(2, 3) - src.get(1, 3) * src.get(2, 1);
		T D_11_32 = src.get(1, 1) * src.get(3, 2) - src.get(1, 2) * src.get(3, 1);
		T D_11_33 = src.get(1, 1) * src.get(3, 3) - src.get(1, 3) * src.get(3, 1);

		T D_02_13 = src.get(0, 2) * src.get(1, 3) - src.get(0, 3) * src.get(1, 2);
		T D_02_23 = src.get(0, 2) * src.get(2, 3) - src.get(0, 3) * src.get(2, 2);
		T D_02_33 = src.get(0, 2) * src.get(3, 3) - src.get(0, 3) * src.get(3, 2);

		T D_01_12 = src.get(0, 1) * src.get(1, 2) - src.get(0, 2) * src.get(1, 1);
		T D_01_13 = src.get(0, 1) * src.get(1, 3) - src.get(0, 3) * src.get(1, 1);
		T D_01_22 = src.get(0, 1) * src.get(2, 2) - src.get(0, 2) * src.get(2, 1);
		T D_01_23 = src.get(0, 1) * src.get(2, 3) - src.get(0, 3) * src.get(2, 1);
		T D_01_32 = src.get(0, 1) * src.get(3, 2) - src.get(0, 2) * src.get(3, 1);
		T D_01_33 = src.get(0, 1) * src.get(3, 3) - src.get(0, 3) * src.get(3, 1);

		vector4<T> col0, col1, col2, col3;

		/*
		*
		* m5 m9	 m13
		* m6 m10 m14
		* m7 m11 m15
		*/
		col0.x = src.get(1, 1) * D_22_33 - src.get(2, 1) * D_12_33 + src.get(3, 1) * D_12_23;
		col0.y = -(src.get(1, 0) * D_22_33 - src.get(2, 0) * D_12_33 + src.get(3, 0) * D_12_23);
		col0.z = src.get(1, 0) * D_21_33 - src.get(2, 0) * D_11_33 + src.get(3, 0) * D_11_23;
		col0.w = -(src.get(1, 0) * D_21_32 - src.get(2, 0) * D_11_32 + src.get(3, 0) * D_11_22);

		col1.x = -(src.get(0, 1) * D_22_33 - src.get(2, 1) * D_02_33 + src.get(3, 1) * D_02_23);
		col1.y = src.get(0, 0) * D_22_33 - src.get(2, 0) * D_02_33 + src.get(3, 0) * D_02_23;
		col1.z = -(src.get(0, 0) * D_21_33 - src.get(2, 0) * D_01_33 + src.get(3, 0) * D_01_23);
		col1.w = src.get(0, 0) * D_21_32 - src.get(2, 0) * D_01_32 + src.get(3, 0) * D_01_22;

		col2.x = src.get(0, 1) * D_12_33 - src.get(1, 1) * D_02_33 + src.get(3, 1) * D_02_13;
		col2.y = -(src.get(0, 0) * D_12_33 - src.get(1, 0) * D_02_33 + src.get(3, 0) * D_02_13);
		col2.z = src.get(0, 0) * D_11_33 - src.get(1, 0) * D_01_33 + src.get(3, 0) * D_01_13;
		col2.w = -(src.get(0, 0) * D_11_32 - src.get(1, 0) * D_01_32 + src.get(3, 0) * D_01_12);

		col3.x = -(src.get(0, 1) * D_12_23 - src.get(1, 1) * D_02_23 + src.get(2, 1) * D_02_13);
		col3.y = src.get(0, 0) * D_12_23 - src.get(1, 0) * D_02_23 + src.get(2, 0) * D_02_13;
		col3.z = -(src.get(0, 0) * D_11_23 - src.get(1, 0) * D_01_23 + src.get(2, 0) * D_01_13);
		col3.w = src.get(0, 0) * D_11_22 - src.get(1, 0) * D_01_22 + src.get(2, 0) * D_01_12;

		result.setColum(col0, 0);
		result.setColum(col1, 1);
		result.setColum(col2, 2);
		result.setColum(col3, 3);

		vector4<T> row0(result.get(0, 0), result.get(0, 1), result.get(0, 2), result.get(0, 3));
		vector4<T> colum0 = src.getColum(0);
		T determinant = dot(row0, colum0);

		assert(determinant != 0);

		T oneOverDeterminant = static_cast<T>(1) / determinant;

		return result * oneOverDeterminant;
	}

	// Space Transformation

	template <typename E>
	Matrix44<E> scale(const mat4f& src, E x, E y, E z) {
		Matrix44<E> result = src;

		E s[3] = { x, y, z };

		for (int col = 0; col < 3; ++col) {
			for (int row = 0; row < 4; ++row) {
				result.m[col * 4 + row] *= s[col];
			}
		}

		return result;
	}

	template <typename E>
	Matrix44<E> translate(const mat4f& src, E x, E y, E z) {
		Matrix44<E> result = src;

		auto col1 = result.getColum(0);
		auto col2 = result.getColum(1);
		auto col3 = result.getColum(2);
		auto col4 = result.getColum(3);

		vector4<E> dstCol = col4 + x * col1 + y * col2 + z * col3;

		result.setColum(dstCol, 3);

		return result;

	}


	template <typename E>
	Matrix44<E> translate(const mat4f& src, vector3<E>& v) {
		return translate(src, v.x, v.y, v.z);
	}


	template <typename E>
	Matrix44<E> rotate(const mat4f& src, float theta, vector3<E>& v) {
		Matrix44<E> rotate;
		Matrix44<E> result;

		float s = std::sin(theta);
		float c = std::cos(theta);

		vector3<E> axis = normalize(v);
		vector3<E> minus_c((1 - c) * axis); // for simplifying the part u_x/y/z * (1 - cos theta)
		rotate.set(0, 0, minus_c[0] * axis[0] + c);
		rotate.set(1, 0, minus_c[0] * axis[1] + s * axis[2]);
		rotate.set(2, 0, minus_c[0] * axis[2] - s * axis[1]);
	
		rotate.set(0, 1, minus_c[1] * axis[0] - s * axis[2]);
		rotate.set(1, 1, minus_c[1] * axis[1] + c);
		rotate.set(2, 1, minus_c[1] * axis[2] + s * axis[0]);

		rotate.set(0, 2, minus_c[2] * axis[0] + s * axis[1]);
		rotate.set(1, 2, minus_c[2] * axis[1] - s * axis[0]);
		rotate.set(2, 2, minus_c[2] * axis[2] + c);

		// src * result

		auto rCol0 = rotate.getColum(0);
		auto rCol1 = rotate.getColum(1);
		auto rCol2 = rotate.getColum(2);
		auto rCol3 = rotate.getColum(3);

		auto srcCol0 = src.getColum(0);
		auto srcCol1 = src.getColum(1);
		auto srcCol2 = src.getColum(2);
		auto srcCol3 = src.getColum(3);

		auto col0 = srcCol0 * rCol0[0] + srcCol1 * rCol0[1] + srcCol2 * rCol0[2];
		auto col1 = srcCol0 * rCol1[0] + srcCol1 * rCol1[1] + srcCol2 * rCol1[2];
		auto col2 = srcCol0 * rCol2[0] + srcCol1 * rCol2[1] + srcCol2 * rCol2[2];
		auto col3 = srcCol3;


		result.setColum(col0, 0);
		result.setColum(col1, 1);
		result.setColum(col2, 2);
		result.setColum(col3, 3);

		return result;
	}


	// Orthogonal Projection
	template<typename T>
	Matrix44<T> orthographic(T left, T right, T bottom, T top, T near, T far) {
		Matrix44<T> result(static_cast<T>(1));

		result.set(0, 0, static_cast<T>(2) / (right - left));
		result.set(0, 3, -(right + left) / (right - left));
		result.set(1, 1, static_cast<T>(2) / (top - bottom));
		result.set(1, 3, -(top + bottom) / (top - bottom));
		result.set(2, 2, -static_cast<T>(2) / (far - near));
		result.set(2, 3, -(far + near) / (far - near));

		return result;
	}

	//Perspective Projection
	// 这里的fov是y方向的fov
	template<typename T>
	Matrix44<T> perspective(T fovy, T aspect, T n, T f) {
		T const tanHalfFovy = std::tan(DEG2RAD(fovy / static_cast<T>(2)));

		Matrix44<T> result(static_cast<T>(0));
		result.set(0, 0, static_cast<T>(1) / (aspect * tanHalfFovy));
		result.set(1, 1, static_cast<T>(1) / (tanHalfFovy));
		result.set(2, 2, -(f + n) / (f - n));
		result.set(2, 3, -static_cast<T>(2) * f * n / (f - n));
		result.set(3, 2, -static_cast<T>(1));

		return result;
	}


	// Screen space transformation
	template<typename T>
	Matrix44<T> screenMatrix(const uint32_t& width, const uint32_t& height) {
		Matrix44<T> result(static_cast<T>(1));

		//x
		result.set(0, 0, static_cast<T>(width) / static_cast<T>(2));
		result.set(0, 3, static_cast<T>(width) / static_cast<T>(2));

		//y
		result.set(1, 1, static_cast<T>(height) / static_cast<T>(2));
		result.set(1, 3, static_cast<T>(height) / static_cast<T>(2));

		//z
		result.set(2, 2, 0.5f);
		result.set(2, 3, 0.5f);

		return result;
	}

}