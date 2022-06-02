#pragma once

#ifndef FIRE_H
#define FIRE_H

#include "Minion.h"

class Fire : public Minion
{
private:
	int toYPos;
public:
	Fire(float fXPos, float fYPos, int toYPos);
	~Fire(void);

	void Update();
	void Draw(SDL_Renderer* renderer, Image* iIMG);
	void collisionWithPlayer(bool TOP);
	void minionPhysics();
};

#endif