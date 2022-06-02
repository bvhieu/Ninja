#pragma once

#ifndef IMAGE_H
#define IMAGE_H

#include "header.h"

class Image
{
private:
	SDL_Texture* tIMG;
	SDL_Rect rRect;

public:
	Image(void);
	Image(std::string fileName, SDL_Renderer* renderer);
	~Image(void);

	void Draw(SDL_Renderer * renderer, int iXOffset, int iYOffset);
	void Draw(SDL_Renderer * renderer, int iXOffset, int iYOffset, bool bRoate);
	void DrawVert(SDL_Renderer * renderer, int iXOffset, int iYOffset);
	void Draw(SDL_Renderer * renderer, SDL_Rect rCrop, SDL_Rect rRect);

	SDL_Texture* getIMG();
	void setIMG(std::string fileName, SDL_Renderer* renderer);
	SDL_Rect getRect();
};

#endif