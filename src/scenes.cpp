/*
 * scene.cpp
 *
 *  Created on: 6 giu 2020
 *      Author: mauro
 */
#include <iostream>

#include "../include/scenes.h"

Scene::Scene(int& idx_scene_) : idx_scene(idx_scene_) {
	if(idx_scene == 1) {
		NUM_OBJECTS = 17;
		area_light_room = 1188;
	}
	else if(idx_scene == 2) {
		NUM_OBJECTS = 17;
		area_light_room = 275;
	}
	else if(idx_scene == 3) {
		NUM_OBJECTS = 18;
		area_light_room = 1680;
	}
	else if(idx_scene == 4) {    // Load with tinyobjectloader
		NUM_OBJECTS = 0;
		area_light_room = 0;
	}
}


std::vector<Hitable*> Scene::get_scene() {
	switch(idx_scene){

		// Scene 1, Box ---------------------------------------------------------------------
		case 1:
			return {
				new Rectangle_xy(1, 99, 0, 81.6, 0, Vec(),Vec(.75, .75, .75)), 			// Front
				new Rectangle_xy(1, 99, 0, 81.6, 170, Vec(), Vec(.75, .75, .75)),		// Back
				new Rectangle_yz(0, 81.6, 0, 170, 1, Vec(), Vec(.25, .75, .25)),	// Left, green
				new Rectangle_yz(0, 81.6, 0, 170, 99, Vec(), Vec(.75, .25, .25)),		// Right, red
				new Rectangle_xz(1, 99, 0, 170, 0, Vec(), Vec(.75, .75, .75)),			// Bottom
				new Rectangle_xz(1, 99, 0, 170, 81.6, Vec(), Vec(.75, .75, .75)),		// Top
				new Rectangle_xz(32, 68, 63, 96, 81.595, Vec(12, 12, 12), Vec()),		// Light 1
				new Rectangle_xy(12, 42, 0, 50, 32, Vec(), Vec(.95,.95,.95)),			// Tall box
				new Rectangle_xy(12, 42, 0, 50, 62, Vec(), Vec(.95,.95,.95)),
				new Rectangle_yz(0, 50, 32, 62, 12, Vec(), Vec(.95,.95,.95)),
				new Rectangle_yz(0, 50, 32, 62, 42 , Vec(), Vec(.95,.95,.95)),
				new Rectangle_xz(12, 42, 32, 62, 50, Vec(), Vec(.95,.95,.95)),			// Short box
				new Rectangle_xy(63, 88, 0, 25, 63, Vec(), Vec(.95,.95,.95)),
				new Rectangle_xy(63, 88, 0, 25, 88, Vec(), Vec(.95,.95,.95)),
				new Rectangle_yz(0, 25, 63, 88, 63, Vec(), Vec(.95,.95,.95)),
				new Rectangle_yz(0, 25, 63, 88, 88, Vec(), Vec(.95,.95,.95)),
				new Rectangle_xz(63, 88, 63, 88, 25, Vec(), Vec(.95,.95,.95))
			};

		// Scene 2, Sunset ---------------------------------------------------------------------
		case 2:
			return {
				new Rectangle_xy(1, 99, 0, 81.6, 0, Vec(),Vec(.75, .75, .75)), 			// Front
				new Rectangle_xy(1, 99, 0, 81.6, 170, Vec(), Vec(.75, .75, .75)),		// Back
				new Rectangle_yz(0, 81.6, 0, 170, 1, Vec(), Vec(.25, .75, .25)),		// Left, green
				new Rectangle_yz(0, 81.6, 0, 170, 99, Vec(), Vec(.75, .25, .25)),		// Right, red
				new Rectangle_xz(1, 99, 0, 170, 0, Vec(), Vec(.75, .75, .75)),			// Bottom
				new Rectangle_xz(1, 99, 0, 170, 81.6, Vec(), Vec(.75, .75, .75)),		// Top
				new Rectangle_xz(88, 99, 63, 88, 0.01, Vec(12, 12, 12), Vec()),			// Light 2
				new Rectangle_xy(12, 42, 0, 50, 32, Vec(), Vec(.95,.95,.95)),			// Tall box
				new Rectangle_xy(12, 42, 0, 50, 62, Vec(), Vec(.95,.95,.95)),
				new Rectangle_yz(0, 50, 32, 62, 12, Vec(), Vec(.95,.95,.95)),
				new Rectangle_yz(0, 50, 32, 62, 42 , Vec(), Vec(.95,.95,.95)),
				new Rectangle_xz(12, 42, 32, 62, 50, Vec(), Vec(.95,.95,.95)),			// Short box
				new Rectangle_xy(63, 88, 0, 25, 63, Vec(), Vec(.95,.95,.95)),
				new Rectangle_xy(63, 88, 0, 25, 88, Vec(), Vec(.95,.95,.95)),
				new Rectangle_yz(0, 25, 63, 88, 63, Vec(), Vec(.95,.95,.95)),
				new Rectangle_yz(0, 25, 63, 88, 88, Vec(), Vec(.95,.95,.95)),
				new Rectangle_xz(63, 88, 63, 88, 25, Vec(), Vec(.95,.95,.95)),
				};

		// Scene 3, Door ---------------------------------------------------------------------
		case 3:
			return {
				new Rectangle_xy(1, 99, 0, 81.6, 0, Vec(),Vec(.75, .75, .75)), 			// Front
				new Rectangle_xy(1, 99, 0, 81.6, 170, Vec(), Vec(.75, .75, .75)),		// Back
				new Rectangle_yz(0, 81.6, 0, 170, 1, Vec(), Vec(.25, .75, .25)),		// Left, green
				new Rectangle_yz(0, 81.6, 0, 170, 99, Vec(), Vec(.75, .25, .25)),		// Right, red
				new Rectangle_xz(1, 99, 0, 170, 0, Vec(), Vec(.75, .75, .75)),			// Bottom
				new Rectangle_xz(1, 99, 0, 170, 81.6, Vec(), Vec(.75, .75, .75)),		// Top
				new Rectangle_yz(0, 60, 62, 90, 1.01, Vec(12,12,12), Vec()),			// Light 3
				new Rectangle_xy(12, 42, 0, 50, 32, Vec(), Vec(.95,.95,.95)),			// Tall box
				new Rectangle_xy(12, 42, 0, 50, 62, Vec(), Vec(.95,.95,.95)),
				new Rectangle_yz(0, 50, 32, 62, 12, Vec(), Vec(.95,.95,.95)),
				new Rectangle_yz(0, 50, 32, 62, 42 , Vec(), Vec(.95,.95,.95)),
				new Rectangle_xz(12, 42, 32, 62, 50, Vec(), Vec(.95,.95,.95)),			// Short box
				new Rectangle_xy(63, 88, 0, 25, 63, Vec(), Vec(.95,.95,.95)),
				new Rectangle_xy(63, 88, 0, 25, 88, Vec(), Vec(.95,.95,.95)),
				new Rectangle_yz(0, 25, 63, 88, 63, Vec(), Vec(.95,.95,.95)),
				new Rectangle_yz(0, 25, 63, 88, 88, Vec(), Vec(.95,.95,.95)),
				new Rectangle_xz(63, 88, 63, 88, 25, Vec(), Vec(.95,.95,.95)),
				new Rectangle_tilted(Vec(1.0,0,60),Vec(10,0,100),Vec(1,60,60),Vec(), Vec(.75, .25, .25))		// Tilted plane
			};

		// Scene 4: load with Tinyobjloader
		case 4:
			std::string filename_obj = "Astronaut.obj";
			return load_obj(filename_obj, NUM_OBJECTS, area_light_room);
		}
}















































