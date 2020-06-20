/*
 * conversions_utils.cpp
 *
 *  Created on: 8 giu 2020
 *      Author: mauro
 */

#include "../include/conversions_utils.h"

// clamp makes sure that the set is bounded (used for radiance() )
float clamp(float x) {
	return x < 0 ? 0 : x > 1 ? 1 : x;
}

// toInt() applies a gamma correction of 2.2, because our screen doesn't show colors linearly
int toInt(float x) {
	return int(pow(clamp(x), 1 / 2.2) * 255 + .5);
}

// Convert spherical coordinates into cartesian
Vec spherToCart(Vec& spher){
	return Vec(sin(spher.y)*sin(spher.z), sin(spher.y)*cos(spher.z), cos(spher.y));
}

// convert Cartesian coordinates into spherical
Vec cartToSpher(Vec& cart){
	return Vec(1, atan((sqrt(cart.x*cart.x + cart.y*cart.y))/cart.z), atan2(cart.x, cart.y));
}


