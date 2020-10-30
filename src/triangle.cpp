/*
 * triangle.cpp
 *
 *  Created on: 28 set 2020
 *      Author: mauro
 */
#include "../include/triangle.h"

// Implementation class of the Triangle objects

// ---------------------------------------------------------------------------

// Triangle
Triangle::Triangle(Vec v0_, Vec v1_, Vec v2_, std::array<float, 2> t0_,  std::array<float, 2> t1_, std::array<float, 2> t2_, Vec e_, Vec c_) : v0(v0_), v1(v1_), v2(v2_), t0(t0_), t1(t1_), t2(t2_),  e(e_), c(c_) {
}

float Triangle::intersect(const Ray &r, Vec& P) const { // returns distance, 0 if no hit
	// Temporary: the normal is computed here
	Vec &n =  (Vec(v0 - v1)%Vec(v0 - v2)).norm();
	n = n.dot(r.d) < 0 ? n : n*-1;

	float d = (n).dot(v0);
	if((r.d).dot(n)!=0){
		float  t =  - ((n).dot(r.o) - d) / ((n).dot(r.d));
		if (t < 0) return false;
		const float& x = r.o.x + r.d.x * t;
		const float& y = r.o.y + r.d.y * t;
		const float& z = r.o.z + r.d.z * t;
		P = Vec(x, y, z);

		Vec C; 	// vector perpendicular to triangle's plane

		 // edge 0
		Vec edge0 = v1 - v0;
		Vec v0p = P - v0;
		C = edge0 % v0p;
		if (n.dot(C) < 0) return 1e20; // P is on the right side

		// edge 1
		Vec edge1 = v2 - v1;
		Vec v1p = P - v1;
		C = edge1 % v1p;
		if (n.dot(C) < 0) return 1e20; // P is on the right side

		// edge 2
		Vec edge2 = v0 - v2;
		Vec v2p = P - v2;
		C = edge2 % v2p;
		if (n.dot(C) < 0) return 1e20; // P is on the right side

		return t; // this ray hits the triangle
	}
	return 1e20;
}

Vec Triangle::normal(const Ray &r, Hit_records &hit) const {
		const Vec& n = (Vec(v0 - v1)%Vec(v0 - v2)).norm();
		hit.c = c;
		hit.e = e;
		return n.dot(r.d) < 0 ? n : n*-1;
	}

std::array<float, 3> Triangle::add_value_color(std::array<float, 3>& x_reduced) const {
	return { x_reduced[0] / 10 * (rand() / float(RAND_MAX)), x_reduced[1] * (rand() / float(RAND_MAX)),
			x_reduced[2]  / 10 * (rand() / float(RAND_MAX)) };
}
