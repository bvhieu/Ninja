#pragma once

#ifndef SPRITE_H
#define SPRITE_H

#include "Image.h"
#include <string>
#include <vector>

class Sprite
{
private:
	std::vector<Image*> tSprite;
	std::vector<unsigned int> iDelay;

	int iCurrentFrame;
	int iStartFrame;
	int iEndFrame;

	unsigned long lTimePassed;

public:
	Sprite(void);
	Sprite(SDL_Renderer* renderer, std::vector<std::string> tSprite, std::vector<unsigned int> iDelay );
	~Sprite(void);

	void Update();

	Image* getTexture();
	Image* getTexture(int iID);
};

#endif

