#include "Dragon.h"
#include "Core.h"
#include "stdlib.h"
#include "time.h"

Dragon::Dragon(float fXPos, float fYPos) {
	this->fXPos = fXPos;
	this->fYPos = fYPos;
	
	this->iHitBoxX = this->iHitBoxY = 64;

	this->iBlockID = 2;

	this->nextFireFrameID = 1;

	this->collisionOnlyWithPlayer = true;

	this->iYFireStart = (int)fYPos - 32;
	this->iFireID = 0;

	srand((unsigned)time(NULL));
}

Dragon::~Dragon(void) {

}

void Dragon::Update() {
	if(!Core::getMap()->getInEvent()) {
			if(nextFireFrameID < 1) {
				createFire();
				nextFireFrameID = 150 + rand() % 115;
			} else
			 if(nextFireFrameID < 30) {
				--nextFireFrameID;
				iBlockID = 3;
			} else {
				--nextFireFrameID;
				iBlockID = 2;
			}
	}
}

void Dragon::Draw(SDL_Renderer* renderer, Image* iIMG) {
	iIMG->Draw(renderer,(int)(fXPos + Core::getMap()->getXPos()), (int)fYPos, false);
}

void Dragon::minionPhysics() {

}

void Dragon::createFire() {
	Core::getMap()->addFire(fXPos - 40, fYPos + 16, iYFireStart + 16 * (rand()%4 + 1) + 8);
	CommonFunctions::getMusic()->PlayChunk(CommonFunctions::getMusic()->cFIRE);
	++iFireID;
}

void Dragon::collisionWithPlayer(bool TOP) {
	Core::getMap()->playerDeath(true, true);
}