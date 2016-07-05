#ifndef _LEMMING_STATE_H_
#define _LEMMING_STATE_H_

#include <map>

//=================================================================
//Possible GameStates
//=================================================================
enum LemmingState {
	LS_NULL = 0,
	LS_STOPPER,
	LS_STEPPER,
	LS_DIGGER,
	LS_EXPLOSIVE,
	LS_MINER,
	LS_CLIMBER,
	LS_TUNNELER,
	LS_FLOATER,
	LS_NUKE,
	LS_MOVING,
	LS_FALLING
};



#endif