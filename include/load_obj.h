/*
 * load_obj.h
 *
 *  Created on: 27 set 2020
 *      Author: mauro
 */
#include "tiny_obj_loader.h"
#include <vector>
#include <string.h>
#include <iostream>
#include <cassert>
#include <array>

#include "vec.h"
#include "triangle.h"
#include "rectangles.h"

std::vector<Hitable*> load_obj(std::string &name_file, int& NUM_OBJECTS, float& area_light_room);

static void PrintInfo(const tinyobj::attrib_t& attrib,
                      const std::vector<tinyobj::shape_t>& shapes,
                      const std::vector<tinyobj::material_t>& materials);
