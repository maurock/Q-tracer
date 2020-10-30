/*
 * hitable.h
 *
 *  Created on: 5 giu 2020
 *      Author: mauro
 */

#ifndef INCLUDE_HITABLE_H_
#define INCLUDE_HITABLE_H_
#include "ray.h"
#include <array>
#include "hit_records.h"
#include <math.h>
#include <map>
#include <vector>

using Action = int;
using Direction = Vec;
extern std::map<Action, Direction> dictAction;
class Hitable {			// a pure virtual function makes sure we always override the function hit
public:
	virtual float intersect(const Ray &r, Vec& P) const = 0;
	virtual Vec normal(const Ray &r, Hit_records &hit) const = 0;
	virtual std::array<float, 6> add_key(Vec &pos, Vec& nl) const;
	virtual std::array<float, 3> add_value_color(std::array<float, 3>& x_reduced) const = 0;
	virtual std::array<float, 73> add_value(std::map<Action, Direction> *dictAction) const;
	Hitable();
};

bool intersect(const Ray &r, float &t, int &id, int& old_id, const int& NUM_OBJECTS, std::vector<Hitable*> rect);
Vec hittingPoint(const Ray &r, int &id, int& old_id, const int& NUM_OBJECTS, std::vector<Hitable*> rect, Hit_records& hit_rec);

#endif /* INCLUDE_HITABLE_H_ */
