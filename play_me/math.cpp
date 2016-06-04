#include "math_3d.h"


inline bool operator < (const v2i& r1, const v2i& r2) {
	return r1.x < r2.x || (r1.x == r2.x && r1.y < r2.y);
}

v2i& v2i::operator = (const v2i& r) {
	x = r.x;
	y = r.y;
	return (*this);
}

v2i& v2i::operator += (const v2i& r) {
	x += r.x;
	y += r.y;
	return (*this);
}

v2i& v2i::operator -= (const v2i& r) {
	x -= r.x;
	y -= r.y;
	return (*this);
}

v2i& v2i::operator *= (const float r) {
	x *= r;
	y *= r;
	return (*this);
}

v2i& v2i::operator /= (const float r) {
	x /= r;
	y /= r;
	return (*this);
}

v2i v2i::operator + (const v2i& r) {
	return v2i (x + r.x, y + r.y);
}

v2i v2i::operator - (const v2i& r) {
	return v2i (x - r.x, y - r.y);
}

v2i v2i::operator - () {
	return v2i (-x, -y);
}

v2i v2i::operator * (const float r) {
	return v2i (x * r, y * r);
}

v2i v2i::operator / (const float r) {
	return v2i (x / r, y / r);
}


v2i::v2i (const v2f &v) {
	x = v.x;
	y = v.y;
}

inline bool operator < (const v2f& r1, const v2f& r2) {
	return r1.x < r2.x || (r1.x == r2.x && r1.y < r2.y);
}

v2f& v2f::operator = (const v2f& r) {
	x = r.x;
	y = r.y;
	return (*this);
}

v2f& v2f::operator += (const v2f& r) {
	x += r.x;
	y += r.y;
	return (*this);
}

v2f& v2f::operator -= (const v2f& r) {
	x -= r.x;
	y -= r.y;
	return (*this);
}

v2f& v2f::operator *= (const float r) {
	x *= r;
	y *= r;
	return (*this);
}

v2f& v2f::operator /= (const float r) {
	x /= r;
	y /= r;
	return (*this);
}

v2f v2f::operator + (const v2f& r) {
	return v2f (x + r.x, y + r.y);
}

v2f v2f::operator - (const v2f& r) {
	return v2f (x - r.x, y - r.y);
}

v2f v2f::operator - () {
	return v2f (-x, -y);
}

v2f v2f::operator * (const float r) {
	return v2f (x * r, y * r);
}

v2f v2f::operator / (const float r) {
	return v2f (x / r, y / r);
}

#include <cmath>

float length (v2i v) {
	return sqrt (v.x * v.x + v.y * v.y);
}
float length (v2f v) {
	return sqrt (v.x * v.x + v.y * v.y);
}

// Matrix4f
void Matrix4f::operator *=(Matrix4f w) {
	Matrix4f res;
    FOR (i, 4) {
        FOR (j, 4) {
            res.m[i][j] = 0;
        }
    }
    FOR (i, 4) {
        FOR (j, 4) {
            FOR (z, 4) {
                res.m[i][j] += me.m[i][z] * w.m[z][j];
            }
        }
    }
    FOR (i, 4) {
        FOR (j, 4) {
            me.m[i][j] = res.m[i][j];
        }
    }
}