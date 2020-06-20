/*
 * conversions_utils.h
 *
 *  Created on: 8 giu 2020
 *      Author: mauro
 */

#ifndef INCLUDE_CONVERSIONS_UTILS_H_
#define INCLUDE_CONVERSIONS_UTILS_H_
#include <math.h>
#include "vec.h"

// clamp makes sure that the set is bounded (used for radiance() )
float clamp(float x);

// toInt() applies a gamma correction of 2.2, because our screen doesn't show colors linearly
int toInt(float x);

// Convert spherical coordinates into cartesian
Vec spherToCart(Vec& spher);

// convert Cartesian coordinates into spherical
Vec cartToSpher(Vec& cart);

#endif /* INCLUDE_CONVERSIONS_UTILS_H_ */
