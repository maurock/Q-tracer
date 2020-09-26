/*
 * image_utils.cpp
 *
 *  Created on: 26 set 2020
 *      Author: mauro
 */

#include "../include/image_utils.h"

// Write the image in .ppm, P3
void write_PPM_P6(std::string &experiment_name, int &s, Vec *c_te,  int& spp_test, int& w_te, int &h_te){
	std::string title_name_p6 = "images/" + experiment_name + "-" + std::to_string(s) + ".ppm";
	char const *ccc_p6  = &title_name_p6[0];
	std::ofstream myfile;
	myfile.open(ccc_p6, std::ios_base::out | std::ios_base::binary);
	myfile << "P6\n" <<  w_te << "\n" <<  h_te << "\n255\n";
	for (int i = 0; i < w_te*h_te; i++) {
		unsigned char c1 = (unsigned char) toInt(c_te[i].x*spp_test/(s+1));
		myfile.write((char*) &c1, sizeof(c1));
		unsigned char c2 = (unsigned char) toInt(c_te[i].y*spp_test/(s+1));
		myfile.write((char*) &c2, sizeof(c2));
		unsigned char c3 = (unsigned char) toInt(c_te[i].z*spp_test/(s+1));
		myfile.write((char*) &c3, sizeof(c3));
	}
	myfile.close();
}

// Write the image in .ppm, P6
void write_PPM_P3(std::string &experiment_name, int &s, Vec *c_te,  int& spp_test, int& w_te, int &h_te){
	std::string title_name = "images/" + experiment_name + "-" + std::to_string(s) + ".ppm";
	char const *ccc  = &title_name[0];
	FILE *f = fopen(ccc , "w"); // Write image to PPM file.
	fprintf(f, "P3\n%d %d\n%d\n", w_te, h_te, 255);
	for (int i = 0; i < w_te * h_te; i++){
		fprintf(f, "%d %d %d ", toInt(c_te[i].x*spp_test/(s+1)), toInt(c_te[i].y*spp_test/(s+1)), toInt(c_te[i].z*spp_test/(s+1)));
	}
}

