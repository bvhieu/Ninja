#pragma once

#ifndef PLAYERDART_H
#define PLAYERDART_H

#include "Minion.h"

class PlayerDart : public Minion
{
private:
	bool bDestroy;
	int destroyFrameID;
public:
	PlayerDart(int X, int Y, bool moveDirection);
	~PlayerDart(void);

	void Update();
	void Draw(SDL_Renderer* rR, Image* iIMG);

	void updateXPos();

	void minionPhysics();

	void collisionWithPlayer(bool TOP);

	void collisionWithAnotherUnit();

	void collisionEffect();

	void setMinionState(int minionState);
};

#endif