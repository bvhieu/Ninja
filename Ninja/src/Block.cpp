#include "Block.h"

Block::Block(void) {}

Block::Block(Sprite *sSprite, bool bCollision){
	this->sSprite = sSprite;
	this->bCollision = bCollision;
}

Block::~Block(void){
	delete sSprite;
}

void Block::Draw(SDL_Renderer *renderer, int iOffsetX, int iOffsetY){
	sSprite->getTexture()->Draw(renderer, iOffsetX, iOffsetY);
}

Sprite *Block::getSprite(){
	return sSprite;
}

bool Block::getCollision(){
	return bCollision;
}
