/*
 * Struct_states.h
 *
 *  Created on: 5 giu 2020
 *      Author: mauro
 */

#ifndef INCLUDE_STRUCT_STATES_H_
#define INCLUDE_STRUCT_STATES_H_
#include <array>

struct Struct_states {				// Store info on old and new state
	std::array<float, 6> old_state;
	int old_action;
	float prob;
	int old_id;
};

#endif /* INCLUDE_STRUCT_STATES_H_ */
