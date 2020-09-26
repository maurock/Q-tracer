/*
 * save_image_utils.h
 *
 *  Created on: 26 set 2020
 *      Author: mauro
 */

#ifndef INCLUDE_IMAGE_UTILS_H_
#define INCLUDE_IMAGE_UTILS_H_
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <iostream>
#include <fstream>
#include "vec.h"
#include "conversions_utils.h"

// Write the image in .ppm, P3
void write_PPM_P3(std::string &experiment_name, int &s, Vec *c_te,  int& spp_test, int& w_te, int &h_te);

// Write the image in .ppm, P6
void write_PPM_P6(std::string &experiment_name, int &s, Vec *c_te,  int& spp_test, int& w_te, int &h_te);

#endif /* INCLUDE_IMAGE_UTILS_H_ */
