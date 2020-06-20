/*
 * vec.cpp
 *
 *  Created on: 9 giu 2020
 *      Author: mauro
 */

#include "../include/vec.h"

Vec normal(Vec &point, Vec &center) {
	return (point - center).norm();
};

Vec getTangent(Vec& normal) {
	Vec new_vector = Vec(normal.x+1, normal.y+1, normal.z+1);		// This cannot be zero, or parallel to the normal.
															// Usually I need to check, but since all my tangents are (0,0,1),
															// (0,1,0) and (1,0,0), I don't need to check in this SPECIFIC CASE.
	return (normal % (new_vector.norm()));
};



