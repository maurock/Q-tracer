/*
 * ray.h
 *
 *  Created on: 5 giu 2020
 *      Author: mauro
 */

#ifndef INCLUDE_RAY_H_
#define INCLUDE_RAY_H_

#include "vec.h"

struct Ray {
	Vec o, d;
	Ray(Vec o_, Vec d_) : o(o_), d(d_) {}
};


#endif /* INCLUDE_RAY_H_ */
