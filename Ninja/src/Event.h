#pragma once

#ifndef Event_H
#define Event_H

#include "header.h"
#include <vector>

class Event
{
private:
	bool bState; // true = OLD, false = NEW
	unsigned int stepID;
public:
	Event(void);
	~Event(void);

	enum animationType {

		eDEATHNOTHING,
		eDEATHTOP,
		eDEATHBOT,
		eNOTHING,

		eLOADINGMENU,
		eGAMEOVER,

	};

	std::vector<animationType> vOLDDir;
	std::vector<int> vOLDLength;

	std::vector<animationType> vNEWDir;
	std::vector<int> vNEWLength;

	void Normal();
	void end();
	int iSpeed;
	void resetData();
	unsigned int iTime;
	int iDelay;
	bool inEvent;
	bool endGame;

	// ----- Methods
	void Draw(SDL_Renderer* renderer);
	void Animation();
};

#endif