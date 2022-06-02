#include "Frog.h"
#include "Core.h"

Frog::Frog(int iX, int iY, int iBlockID, bool moveDirection) {
	this->fXPos = (float)iX;
	this->fYPos = (float)iY;
	this->iBlockID = iBlockID;
	this->moveDirection = moveDirection;
	this->moveSpeed = 1;
}

Frog::~Frog(void) { }

void Frog::Update() {
	if (minionState == 0) {
		updateXPos();
	} else if(minionState == -2) {
		Minion::minionDeathAnimation();
	} else if (SDL_GetTicks() - 500 >= (unsigned)deadTime) {
		minionState = -1;
	}
}

void Frog::Draw(SDL_Renderer* renderer, Image* iIMG) {
	if(minionState != -2) {
		iIMG->Draw(renderer, (int)fXPos + (int)Core::getMap()->getXPos(), (int)fYPos + 2);
	} else {
		iIMG->DrawVert(renderer, (int)fXPos + (int)Core::getMap()->getXPos(), (int)fYPos + 2);
	}
}

void Frog::collisionWithPlayer(bool TOP) {
	if(TOP) {
		if(minionState == 0) {
			minionState = 1;
			iBlockID = 1;
			Core::getMap()->getPlayer()->resetJump();
			Core::getMap()->getPlayer()->startJump(1);
			points(100);
			CommonFunctions::getMusic()->PlayChunk(CommonFunctions::getMusic()->cSTOMP);
		}
	} else {
		Core::getMap()->playerDeath(true, true);
	}
}

void Frog::setMinionState(int minionState) {
	this->minionState = minionState;

	if (this->minionState == 1) {
		deadTime = SDL_GetTicks();
	}

	Minion::setMinionState(minionState);
}