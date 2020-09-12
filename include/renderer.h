/*
 * path_tracer.h
 *
 *  Created on: 10 giu 2020
 *      Author: mauro
 */

#ifndef INCLUDE_RENDERER_H_
#define INCLUDE_RENDERER_H_

#include <math.h>
#include <map>
#include <vector>
#include <iostream>
#include <sstream>
#include <fstream>
#include <istream>
#include <algorithm>
#include "hitable.h"
#include "vec.h"
#include "ray.h"
#include "struct_states.h"
#include "scenes.h"
#include "conversions_utils.h"
#include "sequence.h"


const int dim_action_space = 72;
using Key = std::array<float, 6>;
using QValue = std::array<float, dim_action_space + 1>;
using Action = int;
using Direction = Vec;
using ColorValue = std::array<float, 3>;
using StateAction = std::array<float, 7>;		// Key for map, State + Action for learning rate and counts
using StateActionCount = float;					// Count of state-action pair visits for the learning rate

class Renderer {
public:
	float denom = 1;
	int training, test, action_space_mode;
	virtual Vec radiance(const Ray &r, int &depth, float *path_length, std::map<Key, QValue> *dict, int& counter_red, std::map<Action, Direction> *dictAction,Struct_states &states_rec, std::map<StateAction, StateActionCount> *dictStateAction, float& epsilon, std::vector<Hitable*> rect, const Scene& scene);
	virtual Vec sampling_scattering();
	virtual std::map<Key, QValue>* load_weights(std::string filename);
	virtual float get_denom() {return denom;}
	virtual int get_training() {return training;}
	virtual int get_test() {return test;}
	virtual int get_action_space_mode() {return action_space_mode;}
	virtual void set_training(int training_);
	virtual ~Renderer();
};


class CosineWeighted: public Renderer {
public:
	int training, test;
	CosineWeighted();
	Vec radiance(const Ray &r, int &depth, float *path_length, std::map<Key, QValue> *dict, int& counter_red, std::map<Action, Direction> *dictAction,Struct_states &states_rec, std::map<StateAction, StateActionCount> *dictStateAction, float& epsilon, std::vector<Hitable*> rect, const Scene& scene);
	Vec sampling_scattering(const Vec& nl);
	int get_training() override {return training;}
	int get_test() override {return test;}
};


class ExplicitLight: public Renderer {
public:
	float denom = 2;
	int training, test;
	ExplicitLight();
	Vec radiance(const Ray &r, int &depth,  float *path_length, std::map<Key, QValue> *dict, int& counter_red, std::map<Action, Direction> *dictAction,Struct_states &states_rec, std::map<StateAction, StateActionCount> *dictStateAction, float& epsilon, std::vector<Hitable*> rect, const Scene& scene);
	Vec sampling_scattering(const Vec& hit, const int& idx_scene);
	virtual float get_denom() override {return denom;}
	int get_training() override {return training;}
	int get_test() override {return test;}
};


class RLmethods: public Renderer {
public:
	int training, test, action_space_mode;
	virtual Vec radiance(const Ray &r, int &depth, float *path_length, std::map<Key, QValue> *dict, int& counter_red, std::map<Action, Direction> *dictAction, Struct_states &state_rec, std::map<StateAction, StateActionCount> *dictStateAction, float& epsilon, std::vector<Hitable*> rect, const Scene& scene) = 0;
	virtual void update_Q_table(Key& state, Key& next_state, Hit_records& hit,std::map<Key, QValue> *dict, std::map<Action, Direction> *dictAction, int &old_action, float& BRDF, Vec& nl, Vec& x, float prob, std::map<StateAction, StateActionCount> *dictStateActionCount) = 0;
	virtual Vec sampling_scattering(std::map<Key, QValue> *dict, std::map<Action, Direction> *dictAction, int &id, Vec &x, Vec &nl, Struct_states& states_rec, std::map<StateAction, StateActionCount> *dictStateActionCount, float& epsilon, std::vector<Hitable*> rect, const Scene& scene);
	virtual std::map<Key, QValue>* load_weights(std::string filename) override;
	virtual void compute_Q_prob(const int& action, const std::array<float, dim_action_space + 1>& qvalue,Struct_states& states_rec, const float& total);
	virtual void sample_spher_coord(Vec& spher_coord, const Vec& point_old_coord);
	virtual void update_stateaction_count(const Key& state, const int& action, std::map<StateAction, StateActionCount> *dictStateActionCount, bool sequence_complete);
	virtual int get_training() override {return training;}
	virtual int get_test() override {return test;}
	virtual int get_action_space_mode() override {return action_space_mode;}
	virtual void set_training(int training_) override {training = training_;}
	virtual void compute_action_prob(const int& action, float& prob_patch);
};

class QLearning: public RLmethods {
public:
	QLearning(int training_, int test_, int action_space_mode_);
	Vec radiance(const Ray &r, int &depth, float *path_length, std::map<Key, QValue> *dict, int& counter_red, std::map<Action, Direction> *dictAction, Struct_states &state_rec, std::map<StateAction, StateActionCount> *dictStateAction, float& epsilon, std::vector<Hitable*> rect, const Scene& scene);
	void update_Q_table(Key& state, Key& next_state, Hit_records& hit,std::map<Key, QValue> *dict, std::map<Action, Direction> *dictAction, int &old_action, float& BRDF, Vec& nl, Vec& x, float prob, std::map<StateAction, StateActionCount> *dictStateActionCount);
};


class QPathGuiding: public RLmethods {
public:
	std::vector<Sequence> sequence;
	QPathGuiding(int training_, int test_, int action_space_mode_);
	Vec radiance(const Ray &r, int &depth, float *path_length, std::map<Key, QValue> *dict, int& counter_red, std::map<Action, Direction> *dictAction, Struct_states &state_rec, std::map<StateAction, StateActionCount> *dictStateAction, float& epsilon, std::vector<Hitable*> rect, const Scene& scene);
	void update_Q_table(Key& state, Key& next_state, Hit_records& hit,std::map<Key, QValue> *dict, std::map<Action, Direction> *dictAction, int &old_action, float& BRDF, Vec& nl, Vec& x, float prob, std::map<StateAction, StateActionCount> *dictStateActionCount);
	void update_stateaction_count(const Key& state, const int& action, std::map<StateAction, StateActionCount> *dictStateActionCount, bool sequence_complete) override;
};


class QUpdate_PG: public RLmethods {
public:
	std::vector<Sequence> sequence;
	QUpdate_PG(int training_, int test_, int action_space_mode_);
	Vec radiance(const Ray &r, int &depth, float *path_length, std::map<Key, QValue> *dict, int& counter_red, std::map<Action, Direction> *dictAction, Struct_states &state_rec, std::map<StateAction, StateActionCount> *dictStateAction, float& epsilon, std::vector<Hitable*> rect, const Scene& scene);
	void update_Q_table(Key& state, Key& next_state, Hit_records& hit,std::map<Key, QValue> *dict, std::map<Action, Direction> *dictAction, int &old_action, float& BRDF, Vec& nl, Vec& x, float prob, std::map<StateAction, StateActionCount> *dictStateActionCount);
	void update_stateaction_count(const Key& state, const int& action, std::map<StateAction, StateActionCount> *dictStateActionCount, bool sequence_complete) override;
};


#endif /* INCLUDE_RENDERER_H_ */
