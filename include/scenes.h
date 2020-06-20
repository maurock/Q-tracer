#ifndef INCLUDE_SCENES_H_
#define INCLUDE_SCENES_H_
#include "../include/rectangles.h"
#include "../include/vec.h"
#include <vector>
#include <map>
//----------------------------------------------------------------------------

// Scene 3: Sunset
class Scene{
public:
	int idx_scene;
	Scene(int& idx_scene_);
	std::vector<Hitable*> get_scene();
	Vec light_sampling(const Vec& nl, const Vec& hit);
	int NUM_OBJECTS;
	float area_light_room;
};

#endif /* INCLUDE_SCENES_H_ */
