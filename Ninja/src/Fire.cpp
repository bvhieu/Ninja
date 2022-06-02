#include "Fire.h"
#include "Core.h"

Fire::Fire(float fXPos, float fYPos, int toYPos) {
	this->fXPos = fXPos;
	this->fYPos = fYPos;
	this->toYPos = toYPos;

	this->collisionOnlyWithPlayer = true;

	this->iBlockID = 4;
	this->iHitBoxX = 48;
	this->iHitBoxY = 10;
}

Fire::~Fire(void) {

}

void Fire::Update() {
	fXPos -= 2;

	if(fXPos + iHitBoxX < 0) {
		minionState = -1;
	}

	if(toYPos > fYPos) {
		++fYPos;
	} else if(toYPos < fYPos) {
		--fYPos;
	}
}

void Fire::Draw(SDL_Renderer* renderer, Image* iIMG) {
	iIMG->Draw(renderer, (int)(fXPos + Core::getMap()->getXPos()), (int)fYPos - 4);
}

void Fire::minionPhysics() { }

void Fire::collisionWithPlayer(bool TOP) {
	Core::getMap()->playerDeath(false || true, true);
}