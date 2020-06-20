/*
 * Vec.h
 *
 *  Created on: 5 giu 2020
 *      Author: mauro
 */

#ifndef INCLUDE_VEC_H_
#define INCLUDE_VEC_H_
#import <math.h>

struct Vec {
	float x, y, z;                  // position, also color (r,g,b)
	Vec(float x_ = 0, float y_ = 0, float z_ = 0) {
		x = x_;
		y = y_;
		z = z_;
	}
	Vec operator+(const Vec &b) const {
		return Vec(x + b.x, y + b.y, z + b.z);
	}
	Vec operator-(const Vec &b) const {
		return Vec(x - b.x, y - b.y, z - b.z);
	}
	Vec operator*(float b) const {
		return Vec(x * b, y * b, z * b);
	}
	Vec operator*(double b) const {
		return Vec(x * b, y * b, z * b);
	}
	Vec operator*(int b) const {
		return Vec(x * b, y * b, z * b);
	}

	Vec mult(const Vec &b) const {
		return Vec(x * b.x, y * b.y, z * b.z);
	}
	Vec& norm() {
		return *this = *this * (1 / sqrt(x * x + y * y + z * z));
	}
	float dot(const Vec &b) const {
		return x * b.x + y * b.y + z * b.z;
	}
	Vec operator%(Vec &b) {
		return Vec(y * b.z - z * b.y, z * b.x - x * b.z, x * b.y - y * b.x);
	} // cross
	Vec operator%(const Vec &b) {
		return Vec(y * b.z - z * b.y, z * b.x - x * b.z, x * b.y - y * b.x);
	} // cross
	float magnitude() {
		return sqrt(x * x + y * y + z * z);
	}
};

Vec normal(Vec &point, Vec &center);
Vec getTangent(Vec& normal);


#endif /* INCLUDE_VEC_H_ */
