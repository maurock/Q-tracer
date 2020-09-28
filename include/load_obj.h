/*
 * load_obj.h
 *
 *  Created on: 27 set 2020
 *      Author: mauro
 */
#include "tiny_obj_loader.h"
#include  <string.h>
#include <iostream>
#include <cassert>

void load_obj(std::string &name_file);

static void PrintInfo(const tinyobj::attrib_t& attrib,
                      const std::vector<tinyobj::shape_t>& shapes,
                      const std::vector<tinyobj::material_t>& materials);
