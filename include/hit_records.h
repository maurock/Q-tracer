/*
 * Hit_records.h
 *
 *  Created on: 5 giu 2020
 *      Author: mauro
 */

#ifndef INCLUDE_HITRECORDS_H_
#define INCLUDE_HITRECORDS_H_
#include "Vec.h"
struct Hit_records {		// Store object element
	Vec c;
	Vec e;
	float hit = false;
};

#endif /* INCLUDE_HITRECORDS_H_ */
