/*
 * visualization_util.h
 *
 *  Created on: 7 giu 2020
 *      Author: mauro
 */

#ifndef INCLUDE_VISUALIZATION_HF_H_
#define INCLUDE_VISUALIZATION_HF_H_
#include <map>
#include <array>
#include <vector>
#include <fstream>
#include "vec.h"
#include "hitable.h"
#include "struct_states.h"

// Declaration of helper functions for visualization and plotting purposes.--------------------------------------------------------------

const int dim_action_space = 72;
using Key = std::array<float, 6>;
using QValue = std::array<float, dim_action_space+1>;

inline Vec visualize_states(std::map<Key, QValue> *dict, int id, Vec& x, int& counter_red, Vec& nl, std::vector<Hitable*> rect);
inline Vec visualize_Q_intensity(std::map<Key, QValue> *dict, int id, Vec& x, Vec& nl, std::vector<Hitable*> rect);
inline void plotly_scatter_rays(Struct_states &states_rec, Vec& x, std::array<float,6>& target_state);
inline void plotly_scatter_state(Key& key , Vec& x, std::array<float,6>& target_state);
inline void plotly_scatter_sphere(Key& key, Vec& d, std::array<float,6>& target_state);
inline void plot_Q(Key& state,std::map<Key, QValue> *dict, std::array<float,6>& target_state);
inline int create_state_space(std::map<Key, QValue> *dict, Vec& nl);


#endif /* INCLUDE_VISUALIZATION_HF_H_ */
