/*
 * triangle.h
 *
 *  Created on: 28 set 2020
 *      Author: mauro
 */

#ifndef INCLUDE_TRIANGLE_H_
#define INCLUDE_TRIANGLE_H_
#include <iostream>
#include "hitable.h"

// Triangle
class Triangle: public Hitable {
public:
	Vec v0, v1, v2, e, c;         // emission, color
	std::array<float,2> t0, t1, t2;
	Triangle(Vec v0_, Vec v1_, Vec v2_, std::array<float, 2> t0_,  std::array<float, 2> t1_, std::array<float, 2> t2_, Vec e_, Vec c_);
	float intersect(const Ray &r, Vec& P) const;
	Vec normal(const Ray &r, Hit_records &hit) const;
	std::array<float, 3> add_value_color(std::array<float, 3>& x_reduced) const;
	Vec get_texture_color(Vec &P) const;
};

#endif /* INCLUDE_TRIANGLE_H_ */
