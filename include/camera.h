/*
 * camera.h
 *
 *  Created on: 6 giu 2020
 *      Author: mauro
 */

#ifndef INCLUDE_CAMERA_H_
#define INCLUDE_CAMERA_H_
#include "../include/vec.h"
#include "../include/ray.h"

class Camera {
public:
	Camera(Vec lookfrom, Vec lookat, Vec vup, float vfov, float aspect);
	Ray get_ray(const float& s, const float& t);
	Vec origin;
	Vec lower_left_corner;
	Vec horizontal;
	Vec vertical;
};

#endif /* INCLUDE_CAMERA_H_ */
