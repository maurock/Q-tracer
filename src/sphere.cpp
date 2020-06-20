/*
 * sphere.cpp
 *
 *  Created on: 6 giu 2020
 *      Author: mauro
 */
#include "../include/sphere.h"

Sphere::Sphere(float rad_, Vec p_, Vec e_, Vec c_) :	rad(rad_), p(p_), e(e_), c(c_){}
float Sphere::intersect(const Ray &r) const { // returns distance, 0 if no hit
	Vec op = p - r.o; 	// Solve t^2*d.d + 2*t*(o-p).d + (o-p).(o-p)-R^2 = 0
	float t, eps = 1e-4;
	float b = op.dot(r.d);
	float det = b * b - op.dot(op) + rad * rad;
	if (det < 0)
		return 0;
	else
		det = sqrt(det);
	return (t = b - det) > eps ? t : ((t = b + det) > eps ? t : 0);
}

bool Sphere::intersect(const Vec &v) const { // check if point is on sphere
	return ((v.x - p.x) * (v.x - p.x) + (v.y - p.y) * (v.y - p.y) + (v.z - p.z) * (v.z - p.z) > ((rad * rad) - 5)
			&& (v.x - p.x) * (v.x - p.x) + (v.y - p.y) * (v.y - p.y) + (v.z - p.z) * (v.z - p.z) < ((rad * rad) + 5)) ? true : false;
}

Vec Sphere::normal(const Ray &r, Hit_records &hit, Vec &x) const {
	Vec n = (x - p).norm();						// sphere normal
	hit.c = c;
	hit.e = e;
	return n.dot(r.d) < 0 ? n : n * -1;	// properly orient the normal. If I am inside the sphere, the normal needs to point towards the inside
										// indeed, the angle would be < 90, so dot() < 0. Also, if in a glass it enters or exits
}
