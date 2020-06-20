/*
 * rectangle_xz.cpp
 *
 *  Created on: 5 giu 2020
 *      Author: mauro
 */
#include "../include/rectangles.h"

// Implementation class of the Rectangle objects

// ---------------------------------------------------------------------------

// Rectangle_xz
Rectangle_xz::Rectangle_xz(float x1_, float x2_, float z1_, float z2_, float y_,
		Vec e_, Vec c_) : x1(x1_), x2(x2_), z1(z1_), z2(z2_), y(y_), e(e_), c(c_) {}

float Rectangle_xz::intersect(const Ray &r) const { // returns distance, 0 if no hit
	float t = (y - r.o.y) / r.d.y;		// ray.y = t* dir.y
	const float& x = r.o.x + r.d.x * t;
	const float& z = r.o.z + r.d.z * t;
	if (x < x1 || x > x2 || z < z1 || z > z2 || t < 0) {
		t = 0;
		return 0;
	} else {
		return t;
	}
}

Vec Rectangle_xz::normal(const Ray &r, Hit_records &hit, Vec &x) const {
		Vec n = Vec(0, 1, 0);
		hit.c = c;
		hit.e = e;
		return n.dot(r.d) < 0 ? n : n * -1;
	}

std::array<float, 3> Rectangle_xz::add_value_color(std::array<float, 3>& x_reduced) const {
	return { x_reduced[0] / 10 * (rand() / float(RAND_MAX)), x_reduced[1] * (rand() / float(RAND_MAX)),
			x_reduced[2]  / 10 * (rand() / float(RAND_MAX)) };
}


// --------------------------------------------------------------------------


// Rectangle xy
Rectangle_xy::Rectangle_xy(float x1_, float x2_, float y1_, float y2_, float z_, Vec e_, Vec c_) :
			x1(x1_), x2(x2_), y1(y1_), y2(y2_), z(z_), e(e_), c(c_) {}

float Rectangle_xy::intersect(const Ray &r) const { // returns distance, 0 if no hit
	float t = (z - r.o.z) / r.d.z;
	const float& x = r.o.x + r.d.x * t;
	const float& y = r.o.y + r.d.y * t;
	if (x < x1 || x > x2 || y < y1 || y > y2 || t < 0) {
		t = 0;
		return 0;
	} else {
		return t;
	}
}

Vec Rectangle_xy::normal(const Ray &r, Hit_records &hit, Vec &x) const {
	Vec n = Vec(0, 0, 1);
	hit.c = c;
	hit.e = e;
	return n.dot(r.d) < 0 ? n : n * -1;
}

std::array<float, 3> Rectangle_xy::add_value_color(std::array<float, 3>& x_reduced) const {
	return { x_reduced[0] / 10 * (rand() / float(RAND_MAX)), x_reduced[1] 	/ 10 * (rand() / float(RAND_MAX)),
			x_reduced[2]  * (rand() / float(RAND_MAX)) };
}


// --------------------------------------------------------------------------


// Rectangle YZ

Rectangle_yz::Rectangle_yz(float y1_, float y2_, float z1_, float z2_, float x_,	Vec e_, Vec c_) :
			y1(y1_), y2(y2_), z1(z1_), z2(z2_), x(x_), e(e_), c(c_) {}

float Rectangle_yz::intersect(const Ray &r) const { // returns distance, 0 if no hit
	float t = (x - r.o.x) / r.d.x;
	const float& y = r.o.y + r.d.y * t;
	const float& z = r.o.z + r.d.z * t;
	if (y < y1 || y > y2 || z < z1 || z > z2 || t < 0) {
		t = 0;
		return 0;
	} else {
		//std::cout << x << " " << y << " " << z << std::endl;
		return t;
	}
}

Vec Rectangle_yz::normal(const Ray &r, Hit_records &hit, Vec &x) const {
	Vec n = Vec(1, 0, 0);
	hit.c = c;
	hit.e = e;
	return n.dot(r.d) < 0 ? n : n*-1;
}

std::array<float, 3> Rectangle_yz::add_value_color(std::array<float, 3>& x_reduced) const {
	return { x_reduced[0] * (rand() / float(RAND_MAX)), x_reduced[1] 	/ 10 * (rand() / float(RAND_MAX)),
			x_reduced[2]  / 10 * (rand() / float(RAND_MAX)) };
}


// --------------------------------------------------------------------------


// Rectangle tilted
Rectangle_tilted::Rectangle_tilted(Vec p1_, Vec p2_, Vec p3_, Vec e_, Vec c_) :
		p1(p1_), p2(p2_), p3(p3_), e(e_), c(c_) {}

float Rectangle_tilted::intersect(const Ray &r) const
{
	const Vec& n = (Vec(p1 - p2)%Vec(p1 - p3)).norm();
	// assuming vectors are all normalized
	if((r.d).dot(n)!=0){
		float  t = ((p1-r.o).dot(n))/((r.d).dot(n));
		const float& x = r.o.x + r.d.x * t;
		const float& y = r.o.y + r.d.y * t;
		const float& z = r.o.z + r.d.z * t;
		if(x < p2.x && x > p1.x && z < p2.z && z > p1.z && y < p3.y && y > p1.y && t > 0){
			return t;
		}
	}
	return 0;
}

Vec Rectangle_tilted::normal(const Ray &r, Hit_records &hit, Vec &x) const {
	const Vec& n = (Vec(p1 - p2)%Vec(p1 - p3)).norm();
	hit.c = c;
	hit.e = e;
	return n.dot(r.d) < 0 ? n : n*-1;
}

std::array<float, 3> Rectangle_tilted::add_value_color(std::array<float, 3>& x_reduced) const {
	return { x_reduced[0] * (rand() / float(RAND_MAX)), x_reduced[1] 	/ 10 * (rand() / float(RAND_MAX)),
			x_reduced[2]  / 10 * (rand() / float(RAND_MAX)) };
}
