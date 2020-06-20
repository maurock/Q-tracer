/*
 * sphere.h
 *
 *  Created on: 6 giu 2020
 *      Author: mauro
 */

#ifndef INCLUDE_SPHERE_H_
#define INCLUDE_SPHERE_H_
#include "hitable.h"

class Sphere: public Hitable {
public:
	float rad;       // radius
	Vec p, e, c;      // position, emission, color
	Sphere(float rad_, Vec p_, Vec e_, Vec c_);
	float intersect(const Ray &r) const;
	bool intersect(const Vec &v) const;
	Vec normal(const Ray &r, Hit_records &hit, Vec &x) const;
};

#endif /* INCLUDE_SPHERE_H_ */
