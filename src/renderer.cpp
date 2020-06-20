/*
 * renderer.cpp
 *
 *  Created on: 10 giu 2020
 *      Author: mauro
 */
#include "../include/renderer.h"


Renderer::Renderer() {};
Renderer::~Renderer() {};
Vec Renderer::radiance(const Ray &r, int &depth, float *path_length, std::map<Key, QValue> *dict, int& counter_red, std::map<Action, Direction> *dictAction,Struct_states &states_rec, std::map<StateAction, StateActionCount> *dictStateAction, float& epsilon, std::vector<Hitable*> rect, const Scene& scene) {};
Vec Renderer::sampling_scattering() {};
std::map<Key, QValue>* Renderer::load_weights(std::string filename) {};
void Renderer::set_training(int training_) {};



// Cosine weighted importance sampling -----------------------------------------------------------------------------------------
CosineWeighted::CosineWeighted() : training(0), test(1) {}

Vec CosineWeighted::radiance(const Ray &r, int &depth, float *path_length, std::map<Key, QValue> *dict, int& counter_red, std::map<Action, Direction> *dictAction,Struct_states &state_rec, std::map<StateAction, StateActionCount> *dictStateAction, float& epsilon, std::vector<Hitable*> rect, const Scene& scene){
	Hit_records hit;
	int id = 0;                           // initialize id of intersected object
	Vec x = hittingPoint(r, id, state_rec.old_id, scene.NUM_OBJECTS, rect);            // id calculated inside the function
	int old_id = id;
	state_rec.old_id = id;
	Hitable* &obj = rect[id];				// the hit object
	Vec nl = obj->normal(r, hit, x);
	Vec f = hit.c;							// object color
	float p = f.x > f.y && f.x > f.z ? f.x : f.y > f.z ? f.y : f.z; // max reflectivity (maximum component of r,g,b)
	const float& q = rand() / float(RAND_MAX);
	float PDF_inverse = 1;
	float BRDF = 1;
	if (depth > 10 || !p)  {// Russian Roulette. After 5 bounces, it determines if the ray continues or stops.
		if (q < p) {
			f = f * (1 / p);
		} else {
			return hit.e;
		}
	}
	depth += 1;
	Vec d;
	float t = 0; 	// distance to intersection
	d = sampling_scattering(nl);
	intersect(Ray(x, d.norm()), t, id, old_id, scene.NUM_OBJECTS, rect);
	if(t < 1e20){
		*path_length = *path_length + t;
	}
	return hit.e + f.mult(radiance(Ray(x, d.norm()), depth, path_length, dict, counter_red, dictAction, state_rec, dictStateAction, epsilon, rect, scene)) * PDF_inverse * BRDF;// get color in recursive function
};

Vec CosineWeighted::sampling_scattering(const Vec& nl){
	float r1 = 2 * M_PI * rand() / float(RAND_MAX); //erand48(Xi);		// get random angle
	float r2 = rand() / float(RAND_MAX); //erand48(Xi);			// get random distance from center
	float r2s = sqrt(r2);
	// Create orthonormal coordinate frame for scattered ray
	Vec w = nl;			// w = normal
	Vec u = ((fabs(w.x) > .1 ? Vec(0, 1) : Vec(1)) % w).norm();
	Vec v = w % u;
	return (u * cos(r1) * r2s + v * sin(r1) * r2s + w * sqrt(1 - r2)).norm();
};


// Cosine weighted importance sampling -----------------------------------------------------------------------------------------------
ExplicitLight::ExplicitLight() : training(0), test(1) {}

Vec ExplicitLight::radiance(const Ray &r, int &depth, float *path_length, std::map<Key, QValue> *dict, int& counter_red, std::map<Action, Direction> *dictAction,Struct_states &state_rec, std::map<StateAction, StateActionCount> *dictStateAction, float& epsilon, std::vector<Hitable*> rect, const Scene& scene){
	Hit_records hit;
	int id = 0;                           // initialize id of intersected object
	Vec x = hittingPoint(r, id, state_rec.old_id, scene.NUM_OBJECTS, rect);            // id calculated inside the function
	int old_id = id;
	state_rec.old_id = id;
	Hitable* &obj = rect[id];				// the hit object
	Vec nl = obj->normal(r, hit, x);
	Vec f = hit.c;							// object color
	float p = f.x > f.y && f.x > f.z ? f.x : f.y > f.z ? f.y : f.z; // max reflectivity (maximum component of r,g,b)
	const float& q = rand() / float(RAND_MAX);
	if (depth > 5 || !p)  {// Russian Roulette. After 5 bounces, it determines if the ray continues or stops.
		if (q < p) {
			f = f * (1 / p);
		} else {
			return hit.e;
		}
	}
	depth += 1;
	Vec d;
	Vec d1;
	Vec d2;
	float PDF_inverse = 1;
	float BRDF = 1;
	float PDF_inverse2 = 1;
	float BRDF2 = 1;
	float t = 0; 	// distance to intersection
	// NEXT EVENT ESTIMATION
	CosineWeighted cos_w;
	d1 = cos_w.sampling_scattering(nl);
	intersect(Ray(x, d1.norm()), t, id, old_id, scene.NUM_OBJECTS, rect);
	if(t < 1e20){
		*path_length = *path_length + t;
	}
	if(id != 6){
		d2 = sampling_scattering(x, scene.idx_scene);			// Change according to the SCENE
		if((d2.norm()).dot(nl) > 0){
			intersect(Ray(x, d2.norm()), t, id, old_id, scene.NUM_OBJECTS, rect);
			if(id==6){
				PDF_inverse2 = fabs((scene.area_light_room * d2.norm().dot(Vec(0, 1, 0))) / (t * t));	//PDF = r^2 / (A * cos(theta_light))
				BRDF2 = fabs(d2.norm().dot(nl) / M_PI);
				return hit.e + f.mult(radiance(Ray(x, d1.norm()), depth, path_length, dict, counter_red, dictAction, state_rec, dictStateAction, epsilon, rect, scene)) * PDF_inverse * BRDF +
						 f.mult(radiance(Ray(x, d2.norm()), depth, path_length, dict, counter_red, dictAction, state_rec, dictStateAction, epsilon, rect, scene)) * PDF_inverse2 * BRDF2 ;  // get color in recursi;// get color in recursive function
			}
		}
	}
	return hit.e + f.mult(radiance(Ray(x, d1.norm()), depth, path_length, dict, counter_red, dictAction, state_rec, dictStateAction, epsilon, rect, scene)) * PDF_inverse * BRDF;// get color in recursive function
}

Vec ExplicitLight::sampling_scattering(const Vec& hit, const int& idx_scene) {
	std::map<int, Vec> map = {{1, Vec(32 + rand() * 36 / float(RAND_MAX), 81.6, 63 + rand() * 33 / float(RAND_MAX) )},  // Scene 1, Box
					          {2, Vec(88 + rand() * 11 / float(RAND_MAX), 0.01, 68 + rand() * 15 / float(RAND_MAX))}, 	// Scene 2, Sunset
					          {3, Vec(1.01, 60 * rand() / float(RAND_MAX), 62 + 28 *rand() / float(RAND_MAX) )}};		// Scene 3, Door
	Vec light_vec = map[idx_scene] - hit;
	return light_vec;
}

// Cosine weighted importance sampling -----------------------------------------------------------------------------------------------
QLearning::QLearning(int training_, int test_, int action_space_mode_) : training(training_), test(test_), action_space_mode(action_space_mode_) {};

std::map<Key, QValue>* QLearning::load_weights(std::string filename){
	std::ifstream infile("weights/" + filename + ".txt");
	std::string line;
	std::vector<std::string> results;
	std::array<float, 6> temp_state;
	std::array<float, dim_action_space+1> temp_qvalue;
	std::map<Key, QValue>* dict = new std::map<Key, QValue>;    			// dict position-Qvalue
	while (std::getline(infile, line))
	{
		std::istringstream iss(line);

		while (std::getline(iss, line, ',')) {
			results.push_back(line);
		}
		for(int i=0; i<6; i++){
			temp_state[i]=(float)std::stod(results[i]);
		}
		for(int i=0; i<dim_action_space+1; i++){
			temp_qvalue[i]=(float)std::stod(results[i+6]);
		}
		std::map<Key, QValue> &addrDict = *dict;
		addrDict[temp_state] = temp_qvalue;
		results.clear();
	}
	std::cout << "WEIGHTS LOADED..." << std::endl;
	return dict;
}

Vec QLearning::radiance(const Ray &r, int &depth,float *path_length, std::map<Key, QValue> *dict, int& counter_red, std::map<Action, Direction> *dictAction,Struct_states &state_rec, std::map<StateAction, StateActionCount> *dictStateAction, float& epsilon, std::vector<Hitable*> rect, const Scene& scene){
	Hit_records hit;
	int id = 0;                           // initialize id of intersected object
	Vec x = hittingPoint(r, id, state_rec.old_id, scene.NUM_OBJECTS, rect);            // id calculated inside the function
	int old_id = id;
	state_rec.old_id = id;
	Hitable* &obj = rect[id];				// the hit object
	Vec nl = obj->normal(r, hit, x);
	Key key = rect[id]->add_key(x, nl.norm());	// add state
	std::map<Key, QValue> &addrDict = *dict;
	if (addrDict.count(key) < 1) {
		QValue value = rect[id]->add_value(dictAction);			// Initialize Q-values
		addrDict[key] = value;
	}
	Vec f = hit.c;							// object color
	//float p = f.x > f.y && f.x > f.z ? f.x : f.y > f.z ? f.y : f.z; // max reflectivity (maximum component of r,g,b)
	//const float& q = rand() / float(RAND_MAX);
	if(hit.e.x>1 && depth>1 && this->get_training()==1){      // Light source hit
		float BRDF = std::max({hit.c.x, hit.c.y, hit.c.z})/M_PI;
		update_Q_table(state_rec.old_state, key, hit, dict, dictAction, state_rec.old_action, BRDF , nl, x, state_rec.prob,dictStateAction);
		return hit.e;
	}
	if (depth > 10 || hit.e.x > 1){	// Stop deterministically at 10
		return hit.e;
	}

	depth += 1;
	Vec d;
	float PDF_inverse = 1;
	float BRDF = 1;
	float t = 0; 	// distance to intersection
	if (depth == 1 && this->get_training() == 1) {
		// ------------ TRAINING PHASE, FIRST BOUNCE --------------------------------------------
		d = sampling_scattering(dict, dictAction, id, x, nl, state_rec, dictStateAction, epsilon, rect, scene);
		PDF_inverse = state_rec.prob;		// PDF = 1/24 since the ray can be scattered in one of the 24 areas
		return radiance(Ray(x, d.norm()), depth, path_length, dict, counter_red, dictAction, state_rec, dictStateAction, epsilon, rect, scene);// get color in recursive function
	}
	else if(depth > 1 && this->get_training() == 1){
		BRDF =  std::max({hit.c.x, hit.c.y, hit.c.z})/M_PI;
		update_Q_table(state_rec.old_state, key, hit, dict, dictAction, state_rec.old_action, BRDF, nl, x, state_rec.prob, dictStateAction);
		d = sampling_scattering(dict, dictAction, id, x, nl, state_rec, dictStateAction, epsilon, rect, scene);
		PDF_inverse = state_rec.prob;		// PDF = 1/24 since the ray can be scattered in one of the 24 areas
		return radiance(Ray(x, d.norm()), depth, path_length, dict, counter_red, dictAction, state_rec, dictStateAction, epsilon, rect, scene);
	}
	else{
		// --------------Q-LEARNING, ACTIVE PHASE --------------------------------------------
		d = sampling_scattering(dict, dictAction, id, x, nl, state_rec, dictStateAction, epsilon, rect, scene);
		const float& cos_theta = nl.dot(d.norm());
		PDF_inverse = state_rec.prob;
		BRDF = 1/M_PI;
		intersect(Ray(x, d.norm()), t, id, old_id, scene.NUM_OBJECTS, rect);
		return hit.e + f.mult(radiance(Ray(x, d.norm()), depth, path_length, dict, counter_red, dictAction, state_rec, dictStateAction, epsilon, rect, scene)) * PDF_inverse * BRDF * cos_theta;// get color in recursive function
	}
}

Vec QLearning::sampling_scattering(std::map<Key, QValue> *dict, std::map<Action, Direction> *dictAction, int &id, Vec &x, Vec &nl, Struct_states& states_rec, std::map<StateAction, StateActionCount> *dictStateActionCount, float& epsilon, std::vector<Hitable*> rect, const Scene& scene){
	std::map<Key, QValue> &addrDict = *dict;
	std::map<Action, Direction> &addrDictAction = *dictAction;
	std::map<StateAction, StateActionCount> &addrDictStateActionCount = *dictStateActionCount;
	const Key& state = rect[id]->add_key(x, nl.norm());		// coordinates
	if (addrDict.count(state) < 1) {
		QValue value = rect[id]->add_value(dictAction);			// To initialize Q-values. To return colors, comment this line.
		addrDict[state] = value;
	}
	// Create temporary coordinates system
	Vec& w = nl.norm();
	const Vec& u = getTangent(w).norm();
	const Vec& v = (w % u).norm();

	std::array<float, dim_action_space + 1>& qvalue = addrDict[state];

	Vec point_old_coord;
	int action;
	const float& random = rand() / float(RAND_MAX);

	// Select action, random or proportional to the Q-value function
	if (this->get_training() == 1 && random < epsilon){
		action = (int) 72 * rand() / float(RAND_MAX);
	}else{
		const float& total = qvalue[dim_action_space];
		const float& prob = random*0.9999;
		float cumulativeProbability = 0.0;
		action = 0;
		for (int i=0; i < dim_action_space; i++) {
			cumulativeProbability += qvalue[i]/total;
			if (prob <= cumulativeProbability) {
				action = i;
				break;
			}
		}
		// Normalized probability of the selected action
		compute_Q_prob(action, qvalue, states_rec, total);
	}

	point_old_coord= addrDictAction[action];
	states_rec.old_state = state;
	states_rec.old_action = action;

	// Add StateAction count
	const std::array<float, 7>& stateactionpair = {state[0],state[1],state[2],state[3],state[4],state[5], (float) action};
	if(addrDictStateActionCount.count(stateactionpair) < 1){
		addrDictStateActionCount[stateactionpair] = 0;
	}
	addrDictStateActionCount[stateactionpair] = addrDictStateActionCount[stateactionpair] + 1;

	// Scatter random inside the selected patch, convert to spherical coordinates for semplicity and then back to cartesian
	Vec spher_coord = cartToSpher(point_old_coord);
	// Once an action is selected, sample a random point inside the pathc associated to that action
	sample_spher_coord(spher_coord, point_old_coord);
	point_old_coord = spherToCart(spher_coord);
	return (u*point_old_coord.x  + v*point_old_coord.y  + w*point_old_coord.z); // new_point.x * u + new_point.y * v + new_point.z * w + hitting_point
}

void QLearning::sample_spher_coord(Vec& spher_coord, const Vec& point_old_coord){
	switch(this->action_space_mode){
		case 0:
			spher_coord.z = (0.523*(rand() / float(RAND_MAX)) - 0.261) + spher_coord.z;		// Action 72
			if(point_old_coord.z < 0.167){
				spher_coord.y = 0.16*(rand() / float(RAND_MAX)) + 1.40;		// math done on the notes: theta - 0.168 < theta < theta - 0.168
			}
			else if(point_old_coord.z >= 0.167 && point_old_coord.z < 0.33){
				spher_coord.y = 0.169*(rand() / float(RAND_MAX)) + 1.23;		// theta - 0.192 < theta < theta - 0.192
			}
			else if(point_old_coord.z >= 0.33 && point_old_coord.z < 0.5){
				spher_coord.y = 0.182*(rand() / float(RAND_MAX)) + 1.048;		// theta - 0.192 < theta < theta - 0.192
			}
			else if(point_old_coord.z >= 0.5 && point_old_coord.z < 0.66){
				spher_coord.y = 0.207*(rand() / float(RAND_MAX)) + 0.841;		// theta - 0.192 < theta < theta - 0.192
			}
			else if(point_old_coord.z >= 0.66 && point_old_coord.z < 0.833){
				spher_coord.y = 0.317*(rand() / float(RAND_MAX)) + 0.523;		// theta - 0.192 < theta < theta - 0.192
			}
			else{
				spher_coord.y = 0.523*(rand() / float(RAND_MAX));
			}
			break;
		case 1:
			spher_coord.z = (0.523*(rand() / float(RAND_MAX)) - 0.261) + spher_coord.z;		// Action 72
			if (point_old_coord.z < 0.408) {
				spher_coord.y = 0.42*(rand() / float(RAND_MAX)) + 1.15;    // math done on the notes: theta - 0.168 < theta < theta - 0.168
			}
			else if (point_old_coord.z >= 0.408 && point_old_coord.z < 0.577) {
				spher_coord.y = 0.195*(rand() / float(RAND_MAX)) + 0.955;		// theta - 0.192 < theta < theta - 0.192
			}
			else if (point_old_coord.z >= 0.577 && point_old_coord.z < 0.707) {
				spher_coord.y = 0.170*(rand() / float(RAND_MAX)) + 0.785;		// theta - 0.192 < theta < theta - 0.192
			}
			else if (point_old_coord.z >= 0.707 && point_old_coord.z < 0.816) {
				spher_coord.y = 0.170*(rand() / float(RAND_MAX)) + 0.615;		// theta - 0.192 < theta < theta - 0.192
			}
			else if (point_old_coord.z >= 0.816 && point_old_coord.z < 0.913) {
				spher_coord.y = 0.195*(rand() / float(RAND_MAX)) + 0.420;		// theta - 0.192 < theta < theta - 0.192
			}
			else {
				spher_coord.y = 0.42*(rand() / float(RAND_MAX));
			}
			break;
	}
}

void QLearning::compute_Q_prob(const int& action, const std::array<float, dim_action_space + 1>& qvalue, Struct_states& states_rec, const float& total){
	switch(this->action_space_mode){
		case 0:
			states_rec.prob = (total *  M_PI*2)/(qvalue[action]*(dim_action_space));
			break;
		case 1:
			// New action
			float prob_patch;
			if(action < 12){
				prob_patch = 0.045620675;
			}else if(action >= 12 && action < 24){
				prob_patch = 0.050461491;
			}else if(action >= 24 && action < 36){
				prob_patch = 0.057276365;
			}else if(action >= 36 && action < 48){
				prob_patch = 0.067940351;
			}else if(action >= 48 && action < 60){
				prob_patch = 0.088541589;
			}else{
				prob_patch = 0.213758305;
			}
			states_rec.prob = (total *  prob_patch)/(qvalue[action]);
			break;
		}
}


void QLearning::update_Q_table(Key& state, Key& next_state, Hit_records& hit,std::map<Key, QValue> *dict, std::map<Action, Direction> *dictAction, int &old_action, float& BRDF, Vec& nl, Vec& x, float prob, std::map<StateAction, StateActionCount> *dictStateActionCount){
	std::map<Key, QValue> &addrDict = *dict;
	std::map<Action, Direction> &addrDictAction = *dictAction;
	std::map<StateAction, StateActionCount> &addrDictStateActionCount = *dictStateActionCount;

	float update = 0;
	float& dict_state = addrDict[state][old_action];

	std::array<float, dim_action_space + 1>& dict_next_state = addrDict[next_state];

	float lr = 1  / ((float)addrDictStateActionCount[{state[0], state[1],state[2], state[3],state[4], state[5], (float) old_action}]);
	if (hit.e.x > 5){		// if light
		update = dict_state * (1 - lr) + lr* std::max({hit.e.x, hit.e.y, hit.e.z});
	}else{
		float cumulative_q = 0;
		// Create temporary coordinates system
		Vec &w = nl.norm();
		Vec u = getTangent(w);
		const Vec& v = (u % w);

		switch(this->get_action_space_mode()){
			case 0:
				for(int i=0; i< dim_action_space; i++){
					cumulative_q += dict_next_state[i] * addrDictAction[i].z;
				}
				update = dict_state * (1 - lr) + lr * ((1)/(float) dim_action_space) * cumulative_q *  BRDF * 2 * M_PI;;
				break;
			case 1:
				for(int i=0; i< dim_action_space; i++){
					Vec action_vector = Vec(u*addrDictAction[i].x + v*addrDictAction[i].y + w* addrDictAction[i].z).norm();
					const float& cos_theta_i = w.dot(action_vector);
					// New action
					float prob_cumulative_q = 0;
					if(i < 12){
						prob_cumulative_q = 0.007260756;
					}else if(i >= 12 && i < 24){
						prob_cumulative_q = 0.008031196;
					}else if(i >= 24 && i < 36){
						prob_cumulative_q = 0.009115817;
					}else if(i >= 36 && i < 48){
						prob_cumulative_q = 0.010813043;
					}else if(i >= 48 && i < 60){
						prob_cumulative_q = 0.014091832;
					}else{
						prob_cumulative_q = 0.034020691;
					}
					cumulative_q += (dict_next_state[i] * cos_theta_i)*prob_cumulative_q;
				}
				update = dict_state * (1 - lr) + lr * cumulative_q *  BRDF * 2 * M_PI;
				break;
		}
	}
	addrDict[state][old_action] = update;
	//update total
	float total = 0;
	for(int s=0; s<dim_action_space; s++){
		total += addrDict[state][s];
	}
	addrDict[state][dim_action_space] = total;
}
