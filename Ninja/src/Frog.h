#pragma once

#ifndef FROG_H
#define FROG_H

#include "Minion.h"

class Frog : public Minion
{
public:
	Frog(int iX, int iY, int iBlockID, bool moveDirection);
	~Frog(void);

	void Update();
	void Draw(SDL_Renderer* renderer, Image* iIMG);
	void collisionWithPlayer(bool TOP);
	void setMinionState(int minionState);
};


#endif 