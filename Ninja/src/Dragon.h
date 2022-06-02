#pragma once

#ifndef Dragon_H
#define Dragon_H

#include "Minion.h"

class Dragon : public Minion
{
private:
	int nextFireFrameID;
	int iYFireStart;
	int iFireID;
public:
	Dragon(float fXPos, float fYPos);
	~Dragon(void);

	void Update();
	void Draw(SDL_Renderer* renderer, Image* iIMG);
	void minionPhysics();
	void createFire();
	void collisionWithPlayer(bool TOP);
};

#endif