/*
 * sequence.h
 *
 *  Created on: 20 giu 2020
 *      Author: mauro
 */

#ifndef INCLUDE_SEQUENCE_H_
#define INCLUDE_SEQUENCE_H_
#include <array>

struct Sequence {				// Store info on old and new state
	std::array<float, 6> old_state;
	int old_action;
	float cos_w;
	float BRDF;
};

#endif /* INCLUDE_SEQUENCE_H_ */
