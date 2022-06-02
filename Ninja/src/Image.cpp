#include "Image.h"

Image::Image(void) { }

Image::Image(std::string fileName, SDL_Renderer *renderer){
	setIMG(fileName, renderer);
}

Image::~Image(void){
	SDL_DestroyTexture(tIMG);
}

void Image::Draw(SDL_Renderer *renderer, int iXOffset, int iYOffset){
	rRect.x = iXOffset;
	rRect.y = iYOffset;

	SDL_RenderCopy(renderer, tIMG, NULL, &rRect);
}

void Image::Draw(SDL_Renderer *renderer, int iXOffset, int iYOffset, bool bRotate){
	rRect.x = iXOffset;
	rRect.y = iYOffset;

	if (!bRotate)
	{
		SDL_RenderCopy(renderer, tIMG, NULL, &rRect);
	}
	else
	{
		SDL_RenderCopyEx(renderer, tIMG, NULL, &rRect, 180.0, NULL, SDL_FLIP_VERTICAL);
	}
}

void Image::DrawVert(SDL_Renderer *renderer, int iXOffset, int iYOffset){
	rRect.x = iXOffset;
	rRect.y = iYOffset;

	SDL_RenderCopyEx(renderer, tIMG, NULL, &rRect, 180.0, NULL, SDL_FLIP_HORIZONTAL);
}

void Image::Draw(SDL_Renderer *renderer, SDL_Rect rCrop, SDL_Rect rRect){
	SDL_RenderCopy(renderer, tIMG, &rCrop, &rRect);
}

void Image::setIMG(std::string fileName, SDL_Renderer *renderer){
	fileName = "assets/images/" + fileName + ".bmp";
	SDL_Surface *loadedSurface = SDL_LoadBMP(fileName.c_str());
	SDL_SetColorKey(loadedSurface, SDL_TRUE, SDL_MapRGB(loadedSurface->format, 255, 0, 255));

	tIMG = SDL_CreateTextureFromSurface(renderer, loadedSurface);
	int iWidth, iHeight;

	SDL_QueryTexture(tIMG, NULL, NULL, &iWidth, &iHeight);

	rRect.x = 0;
	rRect.y = 0;
	rRect.w = iWidth;
	rRect.h = iHeight;
	SDL_FreeSurface(loadedSurface);
}

SDL_Texture *Image::getIMG(){
	return tIMG;
}

SDL_Rect Image::getRect(){
	return rRect;
}