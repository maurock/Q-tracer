/*
 * hitable.cpp
 *
 *  Created on: 5 giu 2020
 *      Author: mauro
 */

#include "../include/hitable.h"
#include <vector>

Hitable::Hitable() {};
std::array<float, 6> Hitable::add_key(Vec &pos, Vec& nl) const {
	Vec x_reduced = Vec(ceil((float) pos.x / 5), ceil((float) pos.y/ 5), ceil((float) pos.z / 5 ));
	return { x_reduced.x, x_reduced.y, x_reduced.z, nl.x, nl.y, nl.z};
};
std::array<float, 73> Hitable::add_value(std::map<Action, Direction> *dictAction) const  {
	std::array<float, 73> q_values;
	std::map<Action, Direction> &addrDictAction = *dictAction;
	float total = 0;
	for(int i=0; i <72; i++){
		q_values[i] = addrDictAction[i].z;
		//q_values[i] = 1;
		total += addrDictAction[i].z;
	}
	q_values[72]=total;
	return q_values;
};

bool intersect(const Ray &r, float &t, int &id, int& old_id, const int& NUM_OBJECTS, std::vector<Hitable*> rect) {
	float d;
	float inf = t = 1e20;
	for (int i = 0; i < NUM_OBJECTS; i++) {
		if ((d = rect[i]->intersect(r)) && d < t && i != old_id) {	// Distance of hit point
			t = d;
			id = i;
		}
	}

	// Return the closest intersection, as a bool
	return t < inf;
}
Vec hittingPoint(const Ray &r, int &id, int& old_id, const int& NUM_OBJECTS,std::vector<Hitable*> rect) {
	float t;                             // distance to intersection
	if (!intersect(r, t, id, old_id, NUM_OBJECTS, rect))
		return Vec();

	Vec x = (r.o + r.d * (t - 0.01));// ray intersection point (t calculated in intersect())
	return x ;
}
