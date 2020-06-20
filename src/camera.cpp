/*
 * camera.cpp
 *
 *  Created on: 6 giu 2020
 *      Author: mauro
 */
#include "../include/camera.h"

/*
 Variables:
 lookfrom is the origin
 lookat is the point to look at
 vup, the view up vector to project on the new plane when we incline it.
  We can also tilt the plane
*/
Camera::Camera(Vec lookfrom, Vec lookat, Vec vup, float vfov, float aspect) {// vfov is top to bottom in degrees, field of view on the vertical axis
	Vec w, u, v;
	float theta = vfov * M_PI / 180;	// convert to radiants
	float half_height = tan(theta / 2);
	float half_width = aspect * half_height;
	origin = lookfrom;
	w = (lookat - lookfrom).norm();
	u = (w % vup).norm();
	v = (u % w);

	lower_left_corner = origin - u * half_width - v * half_height + w;
	horizontal = u * (half_width * 2);
	vertical = v * (half_height * 2);
}

Ray Camera::get_ray(const float& s, const float& t) {
	return Ray(origin,
			lower_left_corner + horizontal * s + vertical * t - origin);
}
