#pragma once

#ifndef BLOCK_H
#define BLOCK_H

#include "Image.h"
#include "Sprite.h"

class Block
{
private :
	Sprite* sSprite;

	bool bCollision;

public:
	Block(void);
	Block( Sprite* sSprite, bool bCollision);
	~Block(void);

	void Draw(SDL_Renderer* renderer, int iOffsetX, int iOffsetY);

	Sprite* getSprite();
	bool getCollision();
};

#endif
