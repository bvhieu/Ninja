#include "Minion.h"
#include "Core.h"

Minion::Minion(void) {
	this->minionState = 0;
	this->iHitBoxX = this->iHitBoxY = 32;

	this->killOtherUnits = false;
	this->minionSpawned = false;
	this->collisionOnlyWithPlayer = false;

	this->moveDirection = true;
	this->moveSpeed = 1;

	this->jumpState = 0;
	this->startJumpSpeed = 6.65f;
	this->currentFallingSpeed = 2.2f;

	this->currentJumpSpeed = 0;
	this->jumpDistance = 0;
	this->currentJumpDistance = 0;

	this->deadTime = -1;

	this->onAnotherMinion = false;
}

Minion::~Minion(void) {
	
}

void Minion::Update() { }
void Minion::Draw(SDL_Renderer* renderer, Image* iIMG) { }

bool Minion::updateMinion() {
	if (!minionSpawned) {
		Spawn();
	} else {
		minionPhysics();
	}

	return minionSpawned;
}

void Minion::minionPhysics() {
	if (jumpState == 1) {
		physicsState1();
	} else {
		if (!Core::getMap()->checkCollisionLB((int)fXPos + 2, (int)fYPos + 2, iHitBoxY) && !Core::getMap()->checkCollisionRB((int)fXPos - 2, (int)fYPos + 2, iHitBoxX, iHitBoxY) && !onAnotherMinion) {
			physicsState2();
		} else {
			jumpState = 0;
			onAnotherMinion = false;
		}
	}
}

void Minion::physicsState1() {
	updateYPos(-(int)currentJumpSpeed);
	currentJumpDistance += (int)currentJumpSpeed;

	currentJumpSpeed *= (currentJumpDistance / jumpDistance > 0.75f ? 0.972f : 0.986f);
		
	if (currentJumpSpeed < 2.5f) {
		currentJumpSpeed = 2.5f;
	}

	if (jumpDistance <= currentJumpDistance) {
		jumpState = 2;
	}
}

void Minion::physicsState2() {
	currentFallingSpeed *= 1.06f;

	if (currentFallingSpeed > startJumpSpeed) {
		currentFallingSpeed = startJumpSpeed;
	}

	updateYPos((int)currentFallingSpeed);

	jumpState = 2;

	if (fYPos >= CommonFunctions::GAME_HEIGHT) {
		minionState = -1;
	}
}

void Minion::updateXPos() {
	// ----- LEFT
	if (moveDirection) {
		if (Core::getMap()->checkCollisionLB((int)fXPos - moveSpeed, (int)fYPos - 2, iHitBoxY) || Core::getMap()->checkCollisionLT((int)fXPos - moveSpeed, (int)fYPos + 2)) {
			moveDirection = !moveDirection;
		} else {
			fXPos -= (jumpState == 0 ? moveSpeed : moveSpeed/2.0f);
		}
	}
	// ----- RIGHT
	else {
		if (Core::getMap()->checkCollisionRB((int)fXPos + moveSpeed, (int)fYPos - 2, iHitBoxX, iHitBoxY) || Core::getMap()->checkCollisionRT((int)fXPos + moveSpeed, (int)fYPos + 2, iHitBoxX)) {
			moveDirection = !moveDirection;
		} else {
			fXPos += (jumpState == 0 ? moveSpeed : moveSpeed/2.0f);
		}
	}

	if(fXPos < -iHitBoxX) {
		minionState = -1;
	}
}

void Minion::updateYPos(int iN) {

}

void Minion::collisionEffect() {
	if(minionSpawned)
	moveDirection = !moveDirection;
}

void Minion::collisionWithPlayer(bool TOP) { }

void Minion::Spawn() {
	if ((fXPos >= -Core::getMap()->getXPos() && fXPos <= -Core::getMap()->getXPos() + CommonFunctions::GAME_WIDTH) || (fXPos + iHitBoxX >= -Core::getMap()->getXPos() && fXPos + iHitBoxX <= -Core::getMap()->getXPos() + CommonFunctions::GAME_WIDTH)) {
		minionSpawned = true;
	}
}

void Minion::startJump(int iH) {
	jumpState = 1;
	currentJumpSpeed = startJumpSpeed;
	jumpDistance = 32 * iH + 16.0f;
	currentJumpDistance = 0;
}

void Minion::resetJump() {
	jumpState = 0;
	currentFallingSpeed = 2.7f;
}

void Minion::points(int iPoints) {
	Core::getMap()->getPlayer()->setScore(Core::getMap()->getPlayer()->getScore() + iPoints );
}

void Minion::minionDeathAnimation() {
	fXPos += (moveDirection ? -1.5f : 1.5f);
	fYPos += 2 * (deadTime > 8 ? -1 : deadTime > 2 ? 1 : 4.25f);

	if(deadTime > 0) {
		--deadTime;
	}

	if(fYPos > CommonFunctions::GAME_HEIGHT) {
		minionState = -1;
	}
}

int Minion::getBloockID() {
	return iBlockID;
}

void Minion::setBlockID(int iBlockID) {
	this->iBlockID = iBlockID;
}

int Minion::getXPos() {
	return (int)fXPos;
}

int Minion::getYPos() {
	return (int)fYPos;
}

void Minion::setYPos(int iYPos) {
	this->fYPos = (float)iYPos;
}

int Minion::getMinionState() {
	return minionState;
}

void Minion::setMinionState(int minionState) {
	this->minionState = minionState;
	if(minionState == -2) {
		deadTime = 16;
		fYPos -= iHitBoxY/4;
		points(200);
		collisionOnlyWithPlayer = true;
	}
}