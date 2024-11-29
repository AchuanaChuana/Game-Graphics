#pragma once
#define SQ(x) (x)* (x)

#include<iostream>
#include <cmath>
#include <memory.h>
#include <vector>
#include <limits>
#include <algorithm>

using namespace std;

const float PI = 3.14159265359f;

class Vec2
{
public:
	union
	{
		float v[2];
		struct { float x, y; };
	};

	Vec2() {}

	Vec2(float _x, float _y)
	{
		x = _x;
		y = _y;
	}

	Vec2 operator+(const Vec2& pVec) const
	{
		return Vec2(v[0] + pVec.v[0], v[1] + pVec.v[1]);
	}

	Vec2& operator+=(const Vec2& pVec)
	{
		v[0] += pVec.v[0]; v[1] += pVec.v[1];
		return *this;
	}

	Vec2 operator-(const Vec2& pVec) const
	{
		return Vec2(v[0] - pVec.v[0], v[1] - pVec.v[1]);
	}

	Vec2& operator-=(const Vec2& pVec)
	{
		v[0] -= pVec.v[0]; v[1] -= pVec.v[1];
		return *this;
	}

	Vec2 operator*(const Vec2& pVec) const
	{
		return Vec2(v[0] * pVec.v[0], v[1] * pVec.v[1]);
	}

	Vec2 operator*(const float val) const
	{
		return Vec2(x * val, y * val);
	}

	Vec2& operator*=(const Vec2& pVec)
	{
		v[0] *= pVec.v[0]; v[1] *= pVec.v[1];
		return *this;
	}

	Vec2& operator*=(const float val)
	{
		v[0] *= val; v[1] *= val;
		return *this;
	}

	Vec2 operator/(const Vec2& pVec) const
	{
		return Vec2(v[0] / pVec.v[0], v[1] / pVec.v[1]);
	}

	Vec2 operator/(const float val) const
	{
		float ival = 1.0f / val;
		return Vec2(v[0] * ival, v[1] * ival);
	}

	Vec2& operator/=(const Vec2& pVec)
	{
		v[0] /= pVec.v[0]; v[1] /= pVec.v[1];
		return *this;
	}

	Vec2& operator/=(const float val)
	{
		float ival = 1.0f / val;
		v[0] *= ival; v[1] *= ival;
		return *this;
	}

	Vec2 operator-() const
	{
		return Vec2(-v[0], -v[1]);
	}

	bool operator==(const Vec2& pVec) const
	{
		return (x == pVec.x && y == pVec.y);
	}

	bool operator!=(const Vec2& pVec) const
	{
		return !(*this == pVec);
	}

	float Length() const
	{
		return sqrtf(v[0] * v[0] + v[1] * v[1]);
	}

	float lengthSquare() const
	{
		return v[0] * v[0] + v[1] * v[1];
	}

	Vec2 Normalize() const
	{
		float len = 1.0f / sqrtf(v[0] * v[0] + v[1] * v[1]);
		return Vec2(x * len, y * len);
	}

	float normalize_GetLength()
	{
		float length = sqrtf(v[0] * v[0] + v[1] * v[1]);
		float len = 1.0f / length;
		v[0] *= len; v[1] *= len;
		return length;
	}

	float Dot(const Vec2& pVec) const
	{
		return v[0] * pVec.v[0] + v[1] * pVec.v[1];
	}

	float Cross(const Vec2& v1) const
	{
		return (x * v1.y - y * v1.x);
	}

	float Max() const
	{
		return max(x, y);
	}

	float Min() const
	{
		return min(x, y);
	}
};

class Vec3
{
public:
	union
	{
		float v[3];
		struct { float x, y, z; };
	};

	Vec3(){}

	Vec3(float _x, float _y, float _z)
	{
		x = _x;
		y = _y;
		z = _z;
	}

	Vec3 operator+(const Vec3& pVec) const
	{
		return Vec3(v[0] + pVec.v[0], v[1] + pVec.v[1], v[2] + pVec.v[2]);
	}

	Vec3& operator+=(const Vec3& pVec)
	{
		v[0] += pVec.v[0]; v[1] += pVec.v[1]; v[2] += pVec.v[2];
		return *this;
	}

	Vec3 operator-(const Vec3& pVec) const
	{
		return Vec3(v[0] - pVec.v[0], v[1] - pVec.v[1], v[2] - pVec.v[2]);
	}

	Vec3& operator-=(const Vec3& pVec)
	{
		v[0] -= pVec.v[0]; v[1] -= pVec.v[1]; v[2] -= pVec.v[2];
		return *this;
	}

	Vec3 operator*(const Vec3& pVec) const
	{
		return Vec3(v[0] * pVec.v[0], v[1] * pVec.v[1], v[2] * pVec.v[2]);
	}

	Vec3 operator*(const float val) const
	{
		return Vec3(x*val, y*val,z*val);
	}

	Vec3& operator*=(const Vec3& pVec)
	{
		v[0] *= pVec.v[0]; v[1] *= pVec.v[1]; v[2] *= pVec.v[2];
		return *this;
	}

	Vec3& operator*=(const float val)
	{
		v[0] *= val; v[1] *= val; v[2] *= val;
		return *this;
	}

	Vec3 operator/(const Vec3& pVec) const
	{
		return Vec3(v[0] / pVec.v[0], v[1] / pVec.v[1], v[2] / pVec.v[2]);
	}

	Vec3 operator/(const float val) const
	{
		float ival = 1.0f / val;
		return Vec3(v[0] *ival, v[1] * ival, v[2] * ival);
	}

	Vec3& operator/=(const Vec3& pVec)
	{
		v[0] /= pVec.v[0]; v[1] /= pVec.v[1]; v[2] /= pVec.v[2];
		return *this;
	}

	Vec3& operator/=(const float val)
	{
		float ival = 1.0f / val;
		v[0] *= ival; v[1] *= ival; v[2] *= ival;
		return *this;
	}

	Vec3 operator-() const
	{
		return Vec3(-v[0], -v[1], -v[2]);
	}

	bool operator==(const Vec3& pVec) const
	{
		return (x == pVec.x && y == pVec.y && z == pVec.z);
	}

	bool operator!=(const Vec3& pVec) const
	{
		return !(*this == pVec);
	}

	float Length() const
	{
		return sqrtf(v[0]*v[0]+ v[1] * v[1]+ v[2] * v[2]);
	}

	float lengthSquare()
	{
		return SQ(v[0]) + SQ(v[1]) + SQ(v[2]);
	}

	Vec3 Normalize()
	{
		float len = 1.0f / sqrtf(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);
		return Vec3(x * len, y * len, z * len);
	}

	float normalize_GetLength()
	{
		float length = sqrtf(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);
		float len = 1.0f / length;
		v[0] *= len; v[1] *= len; v[2] *= len;
		return length;
	}

	float Dot(const Vec3& pVec) const
	{
		return v[0] * pVec.v[0] + v[1] * pVec.v[1] + v[2] * pVec.v[2];
	}

	Vec3 Cross(const Vec3& v1)
	{
		return Vec3(v1.v[1] * v[2] - v1.v[2] * v[1],
			v1.v[2] * v[0] - v1.v[0] * v[2],
			v1.v[0] * v[1] - v1.v[1] * v[0]);
	}

	float Max() const
	{
		return max(x, max(y, z));
	}

	float Min() const
	{
		return min(x, min(y, z));
	}

	float getZ()const
	{
		return z;
	}


};

class Vec4
{
public:
	union
	{
		float v[4];
		struct { float x, y, z, w; };
	};

	Vec4() {}

	Vec4(float _x, float _y, float _z, float _w)
	{
		x = _x;
		y = _y;
		z = _z;
		w = _w;
	}

	Vec4 operator+(const Vec4& pVec) const
	{
		return Vec4(v[0] + pVec.v[0], v[1] + pVec.v[1], v[2] + pVec.v[2], v[3] + pVec.v[3]);
	}

	Vec4& operator+=(const Vec4& pVec)
	{
		v[0] += pVec.v[0]; v[1] += pVec.v[1]; v[2] += pVec.v[2]; v[3] += pVec.v[3];
		return *this;
	}

	Vec4 operator-(const Vec4& pVec) const
	{
		return Vec4(v[0] - pVec.v[0], v[1] - pVec.v[1], v[2] - pVec.v[2], v[3] - pVec.v[3]);
	}

	Vec4& operator-=(const Vec4& pVec)
	{
		v[0] -= pVec.v[0]; v[1] -= pVec.v[1]; v[2] -= pVec.v[2]; v[3] -= pVec.v[3];
		return *this;
	}

	Vec4 operator*(const Vec4& pVec) const
	{
		return Vec4(v[0] * pVec.v[0], v[1] * pVec.v[1], v[2] * pVec.v[2], v[3] * pVec.v[3]);
	}

	Vec4 operator*(const float val) const
	{
		return Vec4(x * val, y * val, z * val, w * val);
	}

	Vec4& operator*=(const Vec4& pVec)
	{
		v[0] *= pVec.v[0]; v[1] *= pVec.v[1]; v[2] *= pVec.v[2]; v[3] *= pVec.v[3];
		return *this;
	}

	Vec4& operator*=(const float val)
	{
		v[0] *= val; v[1] *= val; v[2] *= val; v[3] *= val;
		return *this;
	}

	Vec4 operator/(const Vec4& pVec) const
	{
		return Vec4(v[0] / pVec.v[0], v[1] / pVec.v[1], v[2] / pVec.v[2], v[3] / pVec.v[3]);
	}

	Vec4 operator/(const float val) const
	{
		float ival = 1.0f / val;
		return Vec4(v[0] * ival, v[1] * ival, v[2] * ival, v[3] * ival);
	}

	Vec4& operator/=(const Vec4& pVec)
	{
		v[0] /= pVec.v[0]; v[1] /= pVec.v[1]; v[2] /= pVec.v[2]; v[3] /= pVec.v[3];
		return *this;
	}

	Vec4& operator/=(const float val)
	{
		float ival = 1.0f / val;
		v[0] *= ival; v[1] *= ival; v[2] *= ival; v[3] *= ival;
		return *this;
	}

	Vec4 operator-() const
	{
		return Vec4(-v[0], -v[1], -v[2], -v[3]);
	}

	Vec4 homoDivide() const
	{
		if (w == 0.0f)
		{
			cout << "Division by zero: w component is zero." << endl;
		}
		float W = 1.0f / w;
		return Vec4(x * W, y * W, z * W, 1.0f);
	}

	bool operator==(const Vec4& pVec) const
	{
		return (x == pVec.x && y == pVec.y && z == pVec.z && w == pVec.w);
	}

	bool operator!=(const Vec4& pVec) const
	{
		return !(*this == pVec);
	}

	float Length() const
	{
		return sqrtf(v[0] * v[0] + v[1] * v[1] + v[2] * v[2] + v[3] * v[3]);
	}

	float LengthnoW() const 
	{
		return sqrtf(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);
	}


	float lengthSquare()
	{
		return SQ(v[0]) + SQ(v[1]) + SQ(v[2]) + SQ(v[3]);
	}

	Vec4 normalize()
	{
		float len = 1.0f / sqrtf(v[0] * v[0] + v[1] * v[1] + v[2] * v[2] + v[3] * v[3]);
		return Vec4(x * len, y * len, z * len, w * len);
	}

	Vec4 normalizenoW()
	{
		float len = 1.0f / sqrtf(v[0] * v[0] + v[1] * v[1] + v[2] * v[2] + v[3] * v[3]);
		return Vec4(x * len, y * len, z * len, 1.0f);
	}

	float normalize_GetLength()
	{
		float length = sqrtf(v[0] * v[0] + v[1] * v[1] + v[2] * v[2] + v[3] * v[3]);
		float len = 1.0f / length;
		v[0] *= len; v[1] *= len; v[2] *= len; v[3] *= len;
		return length;
	}

	float Dot(const Vec4& pVec) const
	{
		return v[0] * pVec.v[0] + v[1] * pVec.v[1] + v[2] * pVec.v[2] + v[3] * pVec.v[3];
	}

	Vec4 Cross(const Vec4& pVec) const
	{
		return Vec4
		(
			y * pVec.z - z * pVec.y,
			z * pVec.x - x * pVec.z,
			x * pVec.y - y * pVec.x,
			0.0f 
		);
	}

	float Max() const
	{
		return max(x, max(y, max(z, w)));
	}

	float Min() const
	{
		return min(x, min(y, min(z, w)));
	}

	Vec2 toVec2() const
	{
		return Vec2(x, y);
	}

	float getZ()const
	{
		return z;
	}

};

float Dot(const Vec3& v1, const Vec3& v2)
{
	return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}

float Dot(const Vec4& v1, const Vec4& v2)
{
	return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z + v1.w * v2.w;
}

Vec4 AddnoW(const Vec4& v1, const Vec4& v2)
{
	return Vec4(v1.x + v2.x, v1.y + v2.y, v1.z + v2.z, 1.0f);
}

Vec4 MinusnoW(const Vec4& v1, const Vec4& v2)
{
	return Vec4(v1.x - v2.x, v1.y - v2.y, v1.z - v2.z, 1.0f);
}

float Cross(const Vec2& v1, const Vec2& v2)
{
	return v1.v[0] * v2.v[1] - v1.v[1] * v2.v[0];
}

Vec3 Cross(const Vec3& v1, const Vec3& v2)
{
	return Vec3(v1.v[1] * v2.v[2] - v1.v[2] * v2.v[1],
		v1.v[2] * v2.v[0] - v1.v[0] * v2.v[2],
		v1.v[0] * v2.v[1] - v1.v[1] * v2.v[0]);
}

Vec4 Cross(const Vec4& v1, const Vec4& v2)
{
	return Vec4(
		v1.v[1] * v2.v[2] - v1.v[2] * v2.v[1], // x
		v1.v[2] * v2.v[0] - v1.v[0] * v2.v[2], // y
		v1.v[0] * v2.v[1] - v1.v[1] * v2.v[0], // z
		0.0f                                  // w ???? 0
	);
}

Vec3 Max(const Vec3& v1, const Vec3& v2)
{
	return Vec3(max(v1.v[0], v2.v[0]),
		max(v1.v[1], v2.v[1]),
		max(v1.v[2], v2.v[2]));
}

Vec4 Max(const Vec4& v1, const Vec4& v2)
{
	return Vec4(max(v1.v[0], v2.v[0]),
		max(v1.v[1], v2.v[1]),
		max(v1.v[2], v2.v[2]),
		max(v1.v[3], v2.v[3]));
}

Vec3 Min(const Vec3& v1, const Vec3& v2)
{
	return Vec3(min(v1.v[0], v2.v[0]),
		min(v1.v[1], v2.v[1]),
		min(v1.v[2], v2.v[2]));
}

Vec4 Min(const Vec4& v1, const Vec4& v2)
{
	return Vec4(min(v1.v[0], v2.v[0]),
		min(v1.v[1], v2.v[1]),
		min(v1.v[2], v2.v[2]),
		min(v1.v[3], v2.v[3]));
}

Vec3 Normalize(const Vec3& v)
{
	float len = 1.0f / sqrtf(v.v[0] * v.v[0] + v.v[1] * v.v[1] + v.v[2] * v.v[2]);
	return Vec3(v.x * len, v.y * len, v.z * len);
}

Vec4 Normalize(const Vec4& v)
{
	float len = sqrtf(v.v[0] * v.v[0] + v.v[1] * v.v[1] + v.v[2] * v.v[2] + v.v[3] * v.v[3]);
	if (len > 0.0f)
	{
		len = 1.0f / len; 
		return Vec4(v.v[0] * len, v.v[1] * len, v.v[2] * len, v.v[3] * len);
	}
	return Vec4(0.0f, 0.0f, 0.0f, 0.0f);
}

Vec4 NormalizenoW(const Vec4& v)
{
	float len = sqrtf(v.v[0] * v.v[0] + v.v[1] * v.v[1] + v.v[2] * v.v[2]);
	if (len > 0.0f)
	{
		len = 1.0f / len;
		return Vec4(v.v[0] * len, v.v[1] * len, v.v[2] * len, v.w);
	}
	return Vec4(0.0f, 0.0f, 0.0f, 0.0f);
}


class Matrix33
{
public:
	union
	{
		float a[3][3];
		float m[9];
	};

	void identity()
	{
		memset(m, 0, 9 * sizeof(float));
		m[0] = 1.0f;
		m[4] = 1.0f;
		m[8] = 1.0f;
	}

	Matrix33() { identity(); }

	Matrix33(float* otherMatrix)
	{
		memcpy(&m, otherMatrix, 9 * sizeof(float));
	}

	Matrix33(float m00, float m01, float m02, float m10, float m11, float m12, float m20, float m21, float m22)
	{
		a[0][0] = m00;
		a[0][1] = m01;
		a[0][2] = m02;
		a[1][0] = m10;
		a[1][1] = m11;
		a[1][2] = m12;
		a[2][0] = m20;
		a[2][1] = m21;
		a[2][2] = m22;
	}

	float& operator[](int index)
	{
		return m[index];
	}

	Vec3 mulPoint(const Vec3& v)
	{
		Vec3 v1 = Vec3(
			(v.x * m[0] + v.y * m[1] + v.z * m[2]),
			(v.x * m[3] + v.y * m[4] + v.z * m[5]),
			(v.x * m[6] + v.y * m[7] + v.z * m[8])
		);
		float w = (m[6] * v.x) + (m[7] * v.y) + m[8] * v.z;
		w = 1.0f / w;
		return (v1 * w);
	}

	Vec3 mulVec(const Vec3& v)
	{
		return Vec3(
			(v.x * m[0] + v.y * m[1] + v.z * m[2]),
			(v.x * m[3] + v.y * m[4] + v.z * m[5]),
			(v.x * m[6] + v.y * m[7] + v.z * m[8])
		);
	}

	Matrix33 mul(const Matrix33& matrix) const
	{
		Matrix33 ret;
		ret.m[0] = m[0] * matrix.m[0] + m[3] * matrix.m[1] + m[6] * matrix.m[2];
		ret.m[1] = m[1] * matrix.m[0] + m[4] * matrix.m[1] + m[7] * matrix.m[2];
		ret.m[2] = m[2] * matrix.m[0] + m[5] * matrix.m[1] + m[8] * matrix.m[2];
		ret.m[3] = m[0] * matrix.m[3] + m[3] * matrix.m[4] + m[6] * matrix.m[5];
		ret.m[4] = m[1] * matrix.m[3] + m[4] * matrix.m[4] + m[7] * matrix.m[5];
		ret.m[5] = m[2] * matrix.m[3] + m[5] * matrix.m[4] + m[8] * matrix.m[5];
		ret.m[6] = m[0] * matrix.m[6] + m[3] * matrix.m[7] + m[6] * matrix.m[8];
		ret.m[7] = m[1] * matrix.m[6] + m[4] * matrix.m[7] + m[7] * matrix.m[8];
		ret.m[8] = m[2] * matrix.m[6] + m[5] * matrix.m[7] + m[8] * matrix.m[8];
		return ret;
	}

	Matrix33 operator*(const Matrix33& matrix)
	{
		return mul(matrix);
	}

	static Matrix33 translation(const Vec3& v)
	{
		Matrix33 mat;
		mat.a[0][2] = v.x;
		mat.a[1][2] = v.y;
		mat.a[2][2] = v.z;
		return mat;
	}

	static Matrix33 scaling(const Vec3& v)
	{
		Matrix33 mat;
		mat.m[0] = v.x;
		mat.m[4] = v.y;
		mat.m[8] = v.z;
		return mat;
	}

	static Matrix33 rotate(float theta)
	{
		Matrix33 mat;
		float ct = cosf(theta);
		float st = sinf(theta);
		mat.m[0] = ct;
		mat.m[1] = -st;
		mat.m[3] = st;
		mat.m[4] = ct;
		return mat;
	}

	Matrix33 invert()
	{
		Matrix33 inv;
		float det = m[0] * (m[4] * m[8] - m[5] * m[7]) -
			m[1] * (m[3] * m[8] - m[5] * m[6]) +
			m[2] * (m[3] * m[7] - m[4] * m[6]);
		if (det == 0)
		{
			// Handle this case
		}
		float invDet = 1.0f / det;

		inv.m[0] = (m[4] * m[8] - m[5] * m[7]) * invDet;
		inv.m[1] = (m[2] * m[7] - m[1] * m[8]) * invDet;
		inv.m[2] = (m[1] * m[5] - m[2] * m[4]) * invDet;
		inv.m[3] = (m[5] * m[6] - m[3] * m[8]) * invDet;
		inv.m[4] = (m[0] * m[8] - m[2] * m[6]) * invDet;
		inv.m[5] = (m[2] * m[3] - m[0] * m[5]) * invDet;
		inv.m[6] = (m[3] * m[7] - m[4] * m[6]) * invDet;
		inv.m[7] = (m[1] * m[6] - m[0] * m[7]) * invDet;
		inv.m[8] = (m[0] * m[4] - m[1] * m[3]) * invDet;

		return inv;
	}

	Matrix33 transpose()
	{
		Matrix33 trans;
		trans.a[0][0] = a[0][0];
		trans.a[0][1] = a[1][0];
		trans.a[0][2] = a[2][0];
		trans.a[1][0] = a[0][1];
		trans.a[1][1] = a[1][1];
		trans.a[1][2] = a[2][1];
		trans.a[2][0] = a[0][2];
		trans.a[2][1] = a[1][2];
		trans.a[2][2] = a[2][2];
		return trans;
	}
};

class Matrix44
{
public:
	union
	{
		float a[4][4];
		float m[16];
	};

	void identity()
	{
		memset(m, 0, 16 * sizeof(float));
		m[0] = 1.0f;
		m[5] = 1.0f;
		m[10] = 1.0f;
		m[15] = 1.0f;
	}

	Matrix44() { identity(); }

	Matrix44(float* otherMatrix)
	{
		memcpy(&m, otherMatrix, 16 * sizeof(float));
	}

	Matrix44(float m00,float m01,float m02,float m03, float m10, float m11, float m12, float m13, float m20, float m21, float m22, float m23, float m30, float m31, float m32, float m33)
	{
		a[0][0] = m00;
		a[0][1] = m01;
		a[0][2] = m02;
		a[0][3] = m03;
		a[1][0] = m10;
		a[1][1] = m11;
		a[1][2] = m12;
		a[1][3] = m13;
		a[2][0] = m20;
		a[2][1] = m21;
		a[2][2] = m22;
		a[2][3] = m23;
		a[3][0] = m30;
		a[3][1] = m31;
		a[3][2] = m32;
		a[3][3] = m33;
	}

	float& operator[](int index)
	{
		return m[index];
	}

	Vec3 mulPoint(const Vec3& v)
	{
		Vec3 v1 = Vec3(
			(v.x * m[0] + v.y * m[1] + v.z * m[2]) + m[3],
			(v.x * m[4] + v.y * m[5] + v.z * m[6]) + m[7],
			(v.x * m[8] + v.y * m[9] + v.z * m[10]) + m[11]);
		float w;
		w = (m[12] * v.x) + (m[13] * v.y) + (m[14] * v.z) + m[15];
		w = 1.0f / w;
		return (v1 * w);
	}

	Vec3 mulVec(const Vec3& v)
	{
		return Vec3(
			(v.x * m[0] + v.y * m[1] + v.z * m[2]),
			(v.x * m[4] + v.y * m[5] + v.z * m[6]),
			(v.x * m[8] + v.y * m[9] + v.z * m[10]));
	}

	Matrix44 mul(const Matrix44& matrix) const
	{
		Matrix44 ret;
		ret.m[0] = m[0] * matrix.m[0] + m[4] * matrix.m[1] + m[8] * matrix.m[2] + m[12] * matrix.m[3];
		ret.m[1] = m[1] * matrix.m[0] + m[5] * matrix.m[1] + m[9] * matrix.m[2] + m[13] * matrix.m[3];
		ret.m[2] = m[2] * matrix.m[0] + m[6] * matrix.m[1] + m[10] * matrix.m[2] + m[14] * matrix.m[3];
		ret.m[3] = m[3] * matrix.m[0] + m[7] * matrix.m[1] + m[11] * matrix.m[2] + m[15] * matrix.m[3];
		ret.m[4] = m[0] * matrix.m[4] + m[4] * matrix.m[5] + m[8] * matrix.m[6] + m[12] * matrix.m[7];
		ret.m[5] = m[1] * matrix.m[4] + m[5] * matrix.m[5] + m[9] * matrix.m[6] + m[13] * matrix.m[7];
		ret.m[6] = m[2] * matrix.m[4] + m[6] * matrix.m[5] + m[10] * matrix.m[6] + m[14] * matrix.m[7];
		ret.m[7] = m[3] * matrix.m[4] + m[7] * matrix.m[5] + m[11] * matrix.m[6] + m[15] * matrix.m[7];
		ret.m[8] = m[0] * matrix.m[8] + m[4] * matrix.m[9] + m[8] * matrix.m[10] + m[12] * matrix.m[11];
		ret.m[9] = m[1] * matrix.m[8] + m[5] * matrix.m[9] + m[9] * matrix.m[10] + m[13] * matrix.m[11];
		ret.m[10] = m[2] * matrix.m[8] + m[6] * matrix.m[9] + m[10] * matrix.m[10] + m[14] * matrix.m[11];
		ret.m[11] = m[3] * matrix.m[8] + m[7] * matrix.m[9] + m[11] * matrix.m[10] + m[15] * matrix.m[11];
		ret.m[12] = m[0] * matrix.m[12] + m[4] * matrix.m[13] + m[8] * matrix.m[14] + m[12] * matrix.m[15];
		ret.m[13] = m[1] * matrix.m[12] + m[5] * matrix.m[13] + m[9] * matrix.m[14] + m[13] * matrix.m[15];
		ret.m[14] = m[2] * matrix.m[12] + m[6] * matrix.m[13] + m[10] * matrix.m[14] + m[14] * matrix.m[15];
		ret.m[15] = m[3] * matrix.m[12] + m[7] * matrix.m[13] + m[11] * matrix.m[14] + m[15] * matrix.m[15];
		return ret;
	}

	Matrix44 operator*(const Matrix44& matrix)
	{
		return mul(matrix);
	}

	Vec4 operator*(const Vec4& vec) const
	{
		return Vec4(
			a[0][0] * vec.x + a[0][1] * vec.y + a[0][2] * vec.z + a[0][3] * vec.w,
			a[1][0] * vec.x + a[1][1] * vec.y + a[1][2] * vec.z + a[1][3] * vec.w,
			a[2][0] * vec.x + a[2][1] * vec.y + a[2][2] * vec.z + a[2][3] * vec.w,
			a[3][0] * vec.x + a[3][1] * vec.y + a[3][2] * vec.z + a[3][3] * vec.w
		);
	}

	static Matrix44 translation(const Vec3& v)
	{
		Matrix44 mat;
		mat.a[0][3] = v.x;
		mat.a[1][3] = v.y;
		mat.a[2][3] = v.z;
		return mat;
	}

	static Matrix44 scaling(const Vec3& v)
	{
		Matrix44 mat;
		mat.m[0] = v.x;
		mat.m[5] = v.y;
		mat.m[10] = v.z;
		return mat;
	}

	static Matrix44 rotateX(float theta)
	{
		Matrix44 mat;
		float ct = cosf(theta);
		float st = sinf(theta);
		mat.m[5] = ct;
		mat.m[6] = -st;
		mat.m[9] = st;
		mat.m[10] = ct;
		return mat;
	}

	static Matrix44 rotateY(float theta)
	{
		Matrix44 mat;
		float ct = cosf(theta);
		float st = sinf(theta);
		mat.m[0] = ct;   
		mat.m[2] = st;    
		mat.m[8] = -st;   
		mat.m[10] = ct;  
		return mat;
	}

	static Matrix44 rotateZ(float theta)
	{
		Matrix44 mat;
		float ct = cosf(theta);
		float st = sinf(theta);
		mat.m[0] = ct;    
		mat.m[1] = -st;  
		mat.m[4] = st;   
		mat.m[5] = ct;    
		return mat;
	}

	Matrix44 invert()
	{
		Matrix44 inv;
		inv[0] = m[5] * m[10] * m[15] - m[5] * m[11] * m[14] - m[9] * m[6] * m[15] + m[9] * m[7] * m[14] + m[13] * m[6] * m[11] - m[13] * m[7] * m[10];
		inv[4] = -m[4] * m[10] * m[15] + m[4] * m[11] * m[14] + m[8] * m[6] * m[15] - m[8] * m[7] * m[14] - m[12] * m[6] * m[11] + m[12] * m[7] * m[10];
		inv[8] = m[4] * m[9] * m[15] - m[4] * m[11] * m[13] - m[8] * m[5] * m[15] + m[8] * m[7] * m[13] + m[12] * m[5] * m[11] - m[12] * m[7] * m[9];
		inv[12] = -m[4] * m[9] * m[14] + m[4] * m[10] * m[13] + m[8] * m[5] * m[14] - m[8] * m[6] * m[13] - m[12] * m[5] * m[10] + m[12] * m[6] * m[9];
		inv[1] = -m[1] * m[10] * m[15] + m[1] * m[11] * m[14] + m[9] * m[2] * m[15] - m[9] * m[3] * m[14] - m[13] * m[2] * m[11] + m[13] * m[3] * m[10];
		inv[5] = m[0] * m[10] * m[15] - m[0] * m[11] * m[14] - m[8] * m[2] * m[15] + m[8] * m[3] * m[14] + m[12] * m[2] * m[11] - m[12] * m[3] * m[10];
		inv[9] = -m[0] * m[9] * m[15] + m[0] * m[11] * m[13] + m[8] * m[1] * m[15] - m[8] * m[3] * m[13] - m[12] * m[1] * m[11] + m[12] * m[3] * m[9];
		inv[13] = m[0] * m[9] * m[14] - m[0] * m[10] * m[13] - m[8] * m[1] * m[14] + m[8] * m[2] * m[13] + m[12] * m[1] * m[10] - m[12] * m[2] * m[9];
		inv[2] = m[1] * m[6] * m[15] - m[1] * m[7] * m[14] - m[5] * m[2] * m[15] + m[5] * m[3] * m[14] + m[13] * m[2] * m[7] - m[13] * m[3] * m[6];
		inv[6] = -m[0] * m[6] * m[15] + m[0] * m[7] * m[14] + m[4] * m[2] * m[15] - m[4] * m[3] * m[14] - m[12] * m[2] * m[7] + m[12] * m[3] * m[6];
		inv[10] = m[0] * m[5] * m[15] - m[0] * m[7] * m[13] - m[4] * m[1] * m[15] + m[4] * m[3] * m[13] + m[12] * m[1] * m[7] - m[12] * m[3] * m[5];
		inv[14] = -m[0] * m[5] * m[14] + m[0] * m[6] * m[13] + m[4] * m[1] * m[14] - m[4] * m[2] * m[13] - m[12] * m[1] * m[6] + m[12] * m[2] * m[5];
		inv[3] = -m[1] * m[6] * m[11] + m[1] * m[7] * m[10] + m[5] * m[2] * m[11] - m[5] * m[3] * m[10] - m[9] * m[2] * m[7] + m[9] * m[3] * m[6];
		inv[7] = m[0] * m[6] * m[11] - m[0] * m[7] * m[10] - m[4] * m[2] * m[11] + m[4] * m[3] * m[10] + m[8] * m[2] * m[7] - m[8] * m[3] * m[6];
		inv[11] = -m[0] * m[5] * m[11] + m[0] * m[7] * m[9] + m[4] * m[1] * m[11] - m[4] * m[3] * m[9] - m[8] * m[1] * m[7] + m[8] * m[3] * m[5];
		inv[15] = m[0] * m[5] * m[10] - m[0] * m[6] * m[9] - m[4] * m[1] * m[10] + m[4] * m[2] * m[9] + m[8] * m[1] * m[6] - m[8] * m[2] * m[5];
		float det = m[0] * inv[0] + m[1] * inv[4] + m[2] * inv[8] + m[3] * inv[12];
		if (det == 0) {
			// Handle this case
		}
		det = 1.0 / det;
		for (int i = 0; i < 16; i++) {
			inv[i] = inv[i] * det;
		}
		return inv;
	}

	Matrix44 transpose()
	{
		Matrix44 trans;
		for (int i = 0; i < 4; ++i)
		{
			for (int j = 0; j < 4; ++j)
			{
				trans.a[i][j] = a[j][i];
			}
		}
		return trans;
	}

	static Matrix44 worldtoCam(const Vec4& origin, const Vec4& facing, const Vec4& up)
	{
		Vec4 f = (facing - origin).normalize();
		Vec4 s = f.Cross(up).normalize();
		Vec4 u = s.Cross(f);

		Matrix44 mat;
		mat.identity();
		mat.a[0][0] = s.x;
		mat.a[0][1] = s.y;
		mat.a[0][2] = s.z;
		mat.a[1][0] = u.x;
		mat.a[1][1] = u.y;
		mat.a[1][2] = u.z;
		mat.a[2][0] = -f.x;
		mat.a[2][1] = -f.y;
		mat.a[2][2] = -f.z;
		mat.a[0][3] = -s.Dot(origin);
		mat.a[1][3] = -u.Dot(origin);
		mat.a[2][3] = f.Dot(origin);
		return mat;
	}

	static Matrix44 camtoScreen(float fov, float aspect, float nearP, float farP)
	{
		float fovr = fov * PI / 180.f;
		Matrix44 mat;
		float tanHalfFov = tanf(fovr / 2.0f);
		mat.identity();
		mat.a[0][0] = 1.0f / (aspect * tanHalfFov);
		mat.a[1][1] = 1.0f / tanHalfFov;
		mat.a[2][2] = -(farP + nearP) / (farP - nearP);
		mat.a[2][3] = -(2.0f * farP * nearP) / (farP - nearP);
		mat.a[3][2] = -1.0f;
		mat.a[3][3] = 0.0f;
		return mat;
	}

};

class Color : public Vec4
{
public:
	Color() : Vec4() {}

	Color(float _r, float _g, float _b, float _a = 1.0f) : Vec4(_r, _g, _b, _a) {}

	Color(unsigned char _r, unsigned char _g, unsigned char _b, unsigned char _a = 255)
	{
		x = _r / 255.0f;
		y = _g / 255.0f;
		z = _b / 255.0f;
		w = _a / 255.0f;
	}

	float& r() { return x; }
	float& g() { return y; }
	float& b() { return z; }
	float& a() { return w; }

	const float& r() const { return x; }
	const float& g() const { return y; }
	const float& b() const { return z; }
	const float& a() const { return w; }

	Color operator+(const Color& colour) const
	{
		return Color(x + colour.x, y + colour.y, z + colour.z, w);
	}

	Color operator*(const Color& colour) const
	{
		return Color(x * colour.x, y * colour.y, z * colour.z, w);
	}

	Color operator*(const float a) const
	{
		return Color(x * a, y * a, z * a, w);
	}

	Color operator/(const float a) const
	{
		float invA = 1.0f / a;
		return Color(x * invA, y * invA, z * invA, w);
	}
};

class shadingFrame33
{
public:
	Vec3 V1; Vec3 V2; Vec3 V2r; Vec3 V3;

	shadingFrame33(){}

	shadingFrame33(Vec3& v)
	{
	   V1=	v.Normalize();
	   if (V1 != Vec3(1, 0, 0))
	  {
		  V2r = Vec3(1, 0, 0);
		  V2 = Normalize(V2r - V1 * Dot(V2r, V1));
		  V3 = Normalize(Cross(V1, V2));
	  }
	  else
	  {
		  V2r = Vec3(0, 1, 0);
		  V2 = Normalize(V2r - V1 * Dot(V2r, V1));
		  V3 = Normalize(Cross(V1, V2));
	  }
	}

	Vec3 forwardTrans(Vec3& vLocal )
	{
		Vec3 vWorld;
		Matrix33 m(V1.x, V2.x, V3.x, V1.y, V2.y, V3.y, V1.z, V2.z, V3.z);
		vWorld = m.mulVec(vLocal);
		return vWorld;
	}

	Vec3 inverseTrans(Vec3& vWorld)
	{
		Vec3 vLocal;
		Matrix33 m(V1.x, V2.x, V3.x, V1.y, V2.y, V3.y, V1.z, V2.z, V3.z);
		Matrix33 minverse = m.transpose();
		vLocal = minverse.mulVec(vWorld);
		return vLocal;
	}
};

class Transform
{
public:
	Matrix44 modelMatrix;
	Matrix44 viewMatrix;
	Matrix44 projectionMatrix;
	Matrix44 viewportMatrix;

	vector<float> zBuffer;
	float viewportWidth;
	float viewportHeight;

	Transform() : modelMatrix(), viewMatrix(), projectionMatrix(), viewportMatrix(), viewportWidth(0), viewportHeight(0) {}

	void setModelMatrix(const Matrix44& matrix)
	{
		modelMatrix = matrix;
	}

	void setViewMatrix(const Vec4& eye, const Vec4& center, const Vec4& up)
	{
		viewMatrix = Matrix44::worldtoCam(eye, center, up);
	}

	void setProjectionMatrix(float fov, float aspect, float nearP, float farP)
	{
		projectionMatrix = Matrix44::camtoScreen(fov, aspect, nearP, farP);
	}

	void setViewportMatrix(float width, float height)
	{
		viewportMatrix.identity();
		viewportMatrix.a[0][0] = width / 2.0f;
		viewportMatrix.a[1][1] = -height / 2.0f;
		viewportMatrix.a[0][3] = width / 2.0f;
		viewportMatrix.a[1][3] = height / 2.0f;
	}

	const Matrix44& getViewMatrix() const
	{
		return viewMatrix;
	}

	const Matrix44& getProjectionMatrix() const
	{
		return projectionMatrix;
	}

	const Matrix44& getViewportMatrix() const
	{
		return viewportMatrix;
	}

	Vec4 transformToWorld(const Vec4& point) const
	{
		return modelMatrix * point;
	}

	Vec4 transformToView(const Vec4& point) const
	{
		return viewMatrix * point;
	}

	Vec4 transformToClip(const Vec4& point) const
	{
		Vec4 l = projectionMatrix * point;
		//cout << l.x << " " << l.y << " " << l.z << " " << l.w << endl;
		return l;
	}

	bool Clipping(const Vec4& point) const
	{
		if (point.x < -point.w || point.x > point.w ||
			point.y < -point.w || point.y > point.w ||
			point.z < -point.w || point.z > point.w)
		{
			return false;
		}
		else  return true;
	}

	Vec4 transformToViewport(const Vec4& point) const
	{
		if (Clipping(point)==false)
		{
		 // cout << "point is out of bound, so it is clipped" << endl;
			return Vec4(0, 0, 0, 0); 
		}
		else
		{ 
		Vec4 transformedPoint = viewportMatrix * point;
		 return transformedPoint.homoDivide();
		}
	}

	Vec4 transformToScreen(const Vec4& point) const
	{
		Vec4 worldPoint = transformToWorld(point);
		Vec4 viewPoint = transformToView(worldPoint);
		Vec4 clipPoint = transformToClip(viewPoint);
		return transformToViewport(clipPoint);
	}

	void initializeZBuffer()
	{
		zBuffer.resize(static_cast<size_t>(viewportWidth * viewportHeight), numeric_limits<float>::infinity());
	}

	void clearZBuffer()
	{
		fill(zBuffer.begin(), zBuffer.end(), numeric_limits<float>::infinity());
	}

	bool renderZBuffer(const Vec4& screenPoint)
	{
		int x = static_cast<int>(screenPoint.x);
		int y = static_cast<int>(screenPoint.y);
		float z = screenPoint.z;

		if (x < 0 || x >= viewportWidth || y < 0 || y >= viewportHeight)
		{
			return false;
		}

		size_t index = static_cast<size_t>(y * viewportWidth + x);

		if (z < zBuffer[index])
		{
			zBuffer[index] = z;
			return true;
		}

		return false;
	}


};




