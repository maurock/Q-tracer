/*
 * rectangle_xz.h
 *
 *  Created on: 5 giu 2020
 *      Author: mauro
 */
#ifndef INCLUDE_RECTANGLES_H_
#define INCLUDE_RECTANGLES_H_
#include <iostream>
#include "hitable.h"


// Rectangle XY
class Rectangle_xy: public Hitable {
public:
	float x1, x2, y1, y2, z;
	Vec e, c;         // emission, color
	Rectangle_xy(float x1_, float x2_, float y1_, float y2_, float z_,	Vec e_, Vec c_);
	float intersect(const Ray &r, Vec& P) const;
	Vec normal(const Ray &r, Hit_records &hit) const;
	float surface() const;
	std::array<float, 3> add_value_color(std::array<float, 3>& x_reduced) const;
};


//----------------------------------------------------------------

// Rectangle XZ
class Rectangle_xz: public Hitable {
public:
	float x1, x2, z1, z2, y;
	Vec e, c;         // emission, color
	Rectangle_xz(float x1_, float x2_, float z1_, float z2_, float y_,
			Vec e_, Vec c_);
	float intersect(const Ray &r, Vec& P) const;
	Vec normal(const Ray &r, Hit_records &hit) const;
	float surface() const;
	std::array<float, 3> add_value_color(std::array<float, 3>& x_reduced) const;
};


//----------------------------------------------------------------

// Rectangle YZ
class Rectangle_yz: public Hitable {
public:
	float y1, y2, z1, z2, x;
	Vec e, c;         // emission, color
	Rectangle_yz(float y1_, float y2_, float z1_, float z2_, float x_,	Vec e_, Vec c_);
	float intersect(const Ray &r, Vec& P) const;
	Vec normal(const Ray &r, Hit_records &hit) const;
	float surface() const;
	std::array<float, 3> add_value_color(std::array<float, 3>& x_reduced) const;
};


//----------------------------------------------------------------

// Rectangle tilted
class Rectangle_tilted: public Hitable {
public:
	Vec p1, p2, p3;
	Vec e, c;         // emission, color
	Rectangle_tilted(Vec p1_, Vec p2_, Vec p3_, Vec e_, Vec c_);
	float intersect(const Ray &r, Vec& P) const;
	Vec normal(const Ray &r, Hit_records &hit) const;
	std::array<float, 3> add_value_color(std::array<float, 3>& x_reduced) const;
};


#endif /* INCLUDE_RECTANGLES_H_ */
