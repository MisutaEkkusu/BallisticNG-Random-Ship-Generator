#ifndef _SHIPPROTO_H_
#define _SHIPPROTO_H_

#include "SHIPONE.h"

void GenProtOneParams(shipOneParams_t* params);
void MakeProtoOne(shipOne_t* ship, shipOneParams_t params);
void ProtOneNose(shipOne_t* ship, shipOneParams_t params);
void ProtOneMid(shipOne_t* ship, shipOneParams_t params);
void ProtOneTail(shipOne_t* ship, shipOneParams_t params);
void ProtOneWings(shipOne_t* ship, shipOneParams_t params);
#endif
