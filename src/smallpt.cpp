/*
 * smallpt.cpp
 *
 *  Created on: Mar 27, 2019
 *      Author: mauro
 */

#include <stdlib.h>
#include <stdio.h>
#include "../include/ray.h"
#include "../include/vec.h"
#include "../include/struct_states.h"
#include "../include/hit_records.h"
#include "../include/hitable.h"
#include "../include/rectangles.h"
#include "../include/camera.h"
#include "../include/scenes.h"
#include "../include/config_reader.h"
#include "../include/conversions_utils.h"
#include "../include/renderer.h"

using Key = std::array<float, 6>;
using QValue = std::array<float, dim_action_space + 1>;
using StateAction = std::array<float, 7>;		// Key for map, State + Action for learning rate and counts
using StateActionCount = float;					// Count of state-action pair visits for the learning rate


inline void initialize_dictAction(std::map<Action, Direction> *dictAction, int action_space_mode){
	std::map<Action, Direction> &addrDict = *dictAction;
	std::ifstream myFile;
	std::map<int, std::string> file_actions = { {0 , "misc/sphere_point3.csv"}, {1 , "misc/sphere_point4.csv"} };
	myFile.open(file_actions[action_space_mode]);
	std::string x, y, z;
	int count = 0;
	while(myFile.good()){
		if( getline(myFile, x, ',') && getline(myFile, y, ',') &&	getline(myFile, z, '\n')) {
		addrDict[count] = Vec(std::stod(x),std::stod(y),std::stod(z));
		count += 1;
		}
	}
	myFile.close();
}


// LOOPS OVER IMAGE PIXELS, SAVES TO PPM FILE
int main(int argc, char *argv[]) {
	// Read configuration file
	std::string filename = "misc/config.txt";
	ConfigReader config(filename);

	// Load configuration settings
	int w_tr = std::stoi(config.cfg["w_train"]), h_tr = std::stoi(config.cfg["h_train"]), w_te = std::stoi(config.cfg["w_test"]), h_te = std::stoi(config.cfg["h_test"]), spp_train = std::stoi(config.cfg["spp_train"]), spp_test = std::stoi(config.cfg["spp_test"]),
			training = std::stoi(config.cfg["training"]), test = std::stoi(config.cfg["test"]), path_tracing_mode = std::stoi(config.cfg["path_tracing_mode"]), action_space_mode = std::stoi(config.cfg["action_space_mode"]);

	// Initialize required parameters
	// --- Weights
	std::string weight_path = std::string(config.cfg["weight_filename"]), experiment_name = std::string(config.cfg["experiment_name"]);
	// --- Scene
	int idx_scene = std::stoi(config.cfg["scene"]);
	Scene scene(idx_scene);
	std::vector<Hitable*> rect = scene.get_scene();
	// --- Action space
	std::map<Action, Direction>* dictAction = new std::map<Action, Direction>; 	// dict action-direction
	initialize_dictAction(dictAction, action_space_mode);
	// -- LOOKFROM for the Camera
	const Vec LOOKFROM = Vec(50, 40, 168);

	Vec r;					    // Used for colors of samples
	Vec *c_te = new Vec[w_te * h_te]; 	// The image

	std::map<Key, QValue>* dict = new std::map<Key, QValue>;    			// dict position-Qvalue

	// Dictionary for actions


	// Count visit in each state-action pait to adjust the learning rate
	std::map<StateAction, StateActionCount>* dictStateActionCount = new std::map<StateAction, StateActionCount>; 	// dict action-direction

	// Counter for average path length (comparison metric)
	float path_length = 0;
	float* ptrPath_length = &path_length;
	float epsilon;
	int counter_states = 0;
	int counter_red = 0;
	int counter = 0;

	// Choose different renderer based on path tracing mode
	Renderer* renderer;
	switch(path_tracing_mode){
	case 0:
	    renderer = new CosineWeighted();
	    break;
	case 1:
		renderer = new ExplicitLight();
	    break;
	case 2:
		renderer = new QLearning(training, test, action_space_mode);
	    break;
	case 3:
		renderer = new QPathGuiding(training, test, action_space_mode);
	    break;
	case 4:
		renderer = new QUpdate_PG(training, test, action_space_mode);
	    break;
	}
	// Training phase --------------------------------------------------------------------------------------------------
	if (renderer->get_training() == 1){
		epsilon = 1;
		Camera cam(LOOKFROM, Vec(50, 40, 5), Vec(0, 1, 0), 65, float(w_tr) / float(h_tr));
		for (int s = 0; s < spp_train; s++) {		//	scatter rays 100 times more than the size of the screen
			std::cout << "-----------------------" << std::endl << "EPISODE " << (s+1) << std::endl;
			for (int y = 0; y < h_tr; y++) {                 // Loop over image rows
				//fprintf(stderr, "\rTRAINING PHASE: %d Sample, Rendering (%d spp) %5.2f%%", s, num_training_samples, 100. * y / (h - 1));   // Print progress // @suppress("Invalid arguments")
				for (int x = 0; x < w_tr; x++) { // Loop cols. Xi = random seed
					int depth = 0;
					Struct_states state_rec;
					const float& u = float(512*rand() / float(RAND_MAX)) / float(w_tr);
					const float& v = float(512*rand() / float(RAND_MAX)) / float(h_tr);
					Ray d = cam.get_ray(u, v);
					state_rec.old_state = {0,0,0,0,0};
					state_rec.old_action = -1;
					state_rec.old_id = -1;
					state_rec.prob = -1;
					renderer->radiance(Ray(cam.origin, d.d.norm()), depth, ptrPath_length, dict, counter_red, dictAction, state_rec, dictStateActionCount, epsilon, rect, scene); // The average is the same as averaging at the end
					epsilon = - 2 * counter / float(w_tr*h_tr*spp_train) + 1;
					counter +=1;
				}
			}
			std::cout << "Iteration: " << counter << "     Epsilon: " << epsilon <<std::endl;

		}

		std::cout << "NUMBER OF CENTER STATES: " << counter_states << std::endl;

		// Write Q value to a file
		std::ofstream myfile ("weights/" + weight_path + ".txt");
		if (myfile.is_open())
		{
			for(auto const &x : *dict) {
				for(int i=0; i<6; i++){
					myfile << x.first[i] << ",";
				}
				for(int i=0; i< dim_action_space+1; i++){
				  myfile << x.second[i] << ",";
				}
				myfile << std::endl;
			}
			myfile.close();
		}

		std::cout << "FILE CLOSED" << std::endl;
	}

	// Testing phase ------------------------------------------------------------------------------------------------------------------
	if (renderer->get_test() == 1){
		renderer->set_training(0);
		epsilon = 0;
		std::cout << renderer->get_training() << std::endl;
		std::map<Key, QValue> *dict = renderer->load_weights(weight_path);
		Camera cam(LOOKFROM, Vec(50, 40, 5), Vec(0, 1, 0), 65, float(w_te) / float(h_te));
		epsilon = 0;
		for (int s = 0; s < spp_test; s++) {
			std::cout << "Rendering " << s << " / " << spp_test << std::endl;
			int i=0;
			for (int y = 0 ; y < h_te; y++) {                 // Loop over image rows
				//fprintf(stderr, "\rACTIVE PHASE: Rendering (%d spp) %5.2f%%", spp_test, 100. * y / (h_te - 1));
				for (int x = 0; x < w_te; x++) { // Loop cols. Xi = random seed
						// u and v represents the percentage of the horizontal and vertical values
						int depth = 0;
						const float& u = float(x - 0.5 + rand() / float(RAND_MAX)) / float(w_te);
						const float& v = float((h_te - y - 1) - 0.5 + rand() / float(RAND_MAX)) / float(h_te);
						Ray d = cam.get_ray(u, v);
						Struct_states state_rec;
						state_rec.old_state = {0,0,0,0,0};
						state_rec.old_action = -1;
						state_rec.old_id = -1;
						state_rec.prob = -1;
						r = r + renderer->radiance(Ray(cam.origin, d.d.norm()), depth, ptrPath_length, dict, counter_red, dictAction, state_rec, dictStateActionCount, epsilon, rect, scene) * (float) (1 /(renderer->get_denom() * spp_test)); // The average is the same as averaging at the end
					 // Camera rays are pushed ^^^^^ forward to start in interior
						c_te[i] = c_te[i] + Vec(clamp(r.x), clamp(r.y), clamp(r.z));
						i++;
						r = Vec();
				}
			}

			std::string title_name = "images/" + experiment_name + "-" + std::to_string(s) + ".ppm";
			char const *ccc  = &title_name[0];
			FILE *f = fopen(ccc , "w"); // Write image to PPM file.

			//FILE *f = fopen("TITLE" , "w"); // Write image to PPM file.
			fprintf(f, "P3\n%d %d\n%d\n", w_te, h_te, 255);
			for (int i = 0; i < w_te * h_te; i++){
				fprintf(f, "%d %d %d ", toInt(c_te[i].x*spp_test/(s+1)), toInt(c_te[i].y*spp_test/(s+1)), toInt(c_te[i].z*spp_test/(s+1)));
			}
		}
		return 0;
	}
}

