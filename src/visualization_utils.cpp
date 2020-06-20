/*
 * visualization_hf.cpp
 *
 *  Created on: 7 giu 2020
 *      Author: mauro
 */
#include "../include/visualization_utils.h"

// Implementation of helper functions for visualization and plotting purposes.--------------------------------------------------------------


inline Vec visualize_states(std::map<Key, QValue> *dict, int id, Vec& x, int& counter_red, Vec& nl, std::vector<Hitable*> rect){
	// COLOR STATES
	std::map<Key, QValue> &addrDict = *dict;
	Key key = rect[id]->add_key(x, nl.norm());

	// COLOR CENTRE OF STATES
	if( ((x.x > (key[0]*10- 6)) && (x.x < (key[0]*10- 4)) && (x.y > (key[1]*10- 6)) && (x.y < (key[1]*10- 4))) ||
			((x.x > (key[0]*10- 6)) && (x.x < (key[0]*10- 4)) && (x.z > (key[2]*10- 6)) && (x.z < (key[2]*10- 4)))||
			((x.y > (key[1]*10- 6)) && (x.y < (key[1]*10- 4)) && (x.z > (key[2]*10- 6)) && (x.z < (key[2]*10- 4)))){
			counter_red = counter_red + 1;
			return Vec(1,0,0);
	}
	return Vec(addrDict[key][0], addrDict[key][1], addrDict[key][2]);
};

inline Vec visualize_Q_intensity(std::map<Key, QValue> *dict, int id, Vec& x, Vec& nl, std::vector<Hitable*> rect){
	// COLOR STATES
	std::map<Key, QValue> &addrDict = *dict;
	Key key = rect[id]->add_key(x, nl.norm());
	std::array<float, dim_action_space + 1>& qvalue = addrDict[key];

	const float& total = qvalue[dim_action_space];
	if(id==6){
		return Vec(1,1,1);
	}
	float r = 1-(0.49*total/40);
	if (r < 0.51){	r=0.51;}
	float g =  1-total/40;
	float b = 1;
	return Vec(r, g, b );
};

// DRAW SCATTER RAYS IN PLOTLY
inline void plotly_scatter_rays(Struct_states &states_rec, Vec& x, std::array<float,6>& target_state){
	if(states_rec.old_state[0]==target_state[0] && states_rec.old_state[1]==target_state[1] && states_rec.old_state[2]==target_state[2]
		&& states_rec.old_state[3]==target_state[3] && states_rec.old_state[4]==target_state[4] && states_rec.old_state[5]==target_state[5]){
		std::ofstream outfile;
		outfile.open("scattered_rays_target_temp.txt", std::ios_base::app);
		outfile << x.x << "," << x.y << "," << x.z << std::endl;
	}
};

// DRAW SCATTERED STATE IN PLOTLY
inline void plotly_scatter_state(Key& key , Vec& x, std::array<float,6>& target_state){
	if(key[0]==target_state[0] && key[1]==target_state[1] && key[2]==target_state[2]
		&& key[3]==target_state[3] && key[4]==target_state[4] && key[5]==target_state[5]){
		std::ofstream outfile;
		outfile.open("scattered_state_maxAction_temp.txt", std::ios_base::app);
		outfile << x.x << "," << x.y << "," << x.z << std::endl;
	}
};

// DRAW SCATTERED SPHERE IN PLOTLY
inline void plotly_scatter_sphere(Key& key, Vec& d, std::array<float,6>& target_state){
	if(key[0]==target_state[0] && key[1]==target_state[1] && key[2]==target_state[2]
		&& key[3]==target_state[3] && key[4]==target_state[4] && key[5]==target_state[5]){
		Vec sphere = Vec(key[0]*10 - 5 + 5*d.x, key[1]*10 - 5 + 5*d.y,key[2]*10 - 5 + 5*d.z);
		std::ofstream outfile;
		outfile.open("scattered_sphere.txt", std::ios_base::app);
		outfile << sphere.x << "," << sphere.y << "," << sphere.z << std::endl;
	}
};

inline void plot_Q(Key& state,std::map<Key, QValue> *dict, std::array<float,6>& target_state){
	if(state[0]==target_state[0] && state[1]==target_state[1] && state[2]==target_state[2]
			&& state[3]==target_state[3] && state[4]==target_state[4] && state[5]==target_state[5]){

		std::map<Key, QValue> &addrDict = *dict;
		std::ofstream outfile;
		outfile.open("q-plot.txt", std::ios_base::app);
		for(int i=0; i < dim_action_space; i++){
			outfile << addrDict[state][i] << ",";
		}
		outfile << std::endl;
	}
};

/*inline int create_state_space(std::map<Key, QValue> *dict, Vec& nl) {
	std::map<Key, QValue> &addrDict = *dict;
	int count = 0;
	for (int x = 0; x < 100; x++) {
		for (int y = -1; y < 85; y++) {
			for (int z = -1; z < 171; z++) {
				Vec vec = Vec(x, y, z);
				Ray r = Ray(LOOKFROM, (vec - LOOKFROM).norm());
				int id = 0;
				int old_id = 0;				// NOT TRUE, NEED TO PASS STATES_REC
				Vec pos = hittingPoint(r, id, old_id);
				Key key = rect[id]->add_key(pos, nl);
				//QValue value = rect[id]->add_value_color(key);				//To return different colors for each state. If you don't want colors,
																			//comment this
//				if (addrDict.count(key) < 1) {
//					QValue value = rect[id]->add_value();			// To initialize Q-values. To return colors, comment this line.
//					addrDict[key] = value;							// need to add dictAction
//					count += 1;
//				}
			}
		}
	}
	return count;
};*/

