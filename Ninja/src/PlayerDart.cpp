#include "PlayerDart.h"
#include "Core.h"

PlayerDart::PlayerDart(int iXPos, int iYPos, bool moveDirection)
{
	this->fXPos = (float)iXPos;
	this->fYPos = (float)iYPos;

	this->currentYPos = this->fYPos;

	this->moveDirection = moveDirection;

	this->killOtherUnits = true;

	this->moveSpeed = 8;

	this->iBlockID = 5;

	this->iHitBoxX = 16;
	this->iHitBoxY = 16;

	this->bDestroy = false;

	this->destroyFrameID = 10;
}

PlayerDart::~PlayerDart(void)
{
}

void PlayerDart::Update()
{

	if (fXPos < fXPos + 59)
	{
		++fYPos;
	}
	else
	{
		bDestroy = true;
	}
	//
	if (bDestroy)
	{
		if (destroyFrameID < 0)
		{
			this->iBlockID = 6;
		}
		else
		{
			minionState = -2;
		}
		// --destroyFrameID;
	}
	else
	{
		updateXPos();
	}
	--destroyFrameID;
}

void PlayerDart::Draw(SDL_Renderer *renderer, Image *iIMG)
{
	if (!bDestroy)
	{
		iIMG->Draw(renderer, (int)fXPos + (int)Core::getMap()->getXPos(), currentYPos, !moveDirection);
	}
	else
	{
		iIMG->Draw(renderer, (int)fXPos + (int)Core::getMap()->getXPos() - (moveDirection ? 16 : 0), currentYPos, !moveDirection);
	}
}

void PlayerDart::updateXPos()
{
	// ----- LEFT
	if (moveDirection)
	{
		if (Core::getMap()->checkCollisionLB((int)fXPos - moveSpeed, currentYPos, iHitBoxY))
		{
			bDestroy = true;
			collisionOnlyWithPlayer = true;
		}
		else
		{
			fXPos -= moveSpeed;
		}
	}
	// ----- RIGHT
	else
	{
		if (Core::getMap()->checkCollisionRB((int)fXPos + moveSpeed, currentYPos, iHitBoxX, iHitBoxY))
		{
			bDestroy = true;
			collisionOnlyWithPlayer = true;
		}
		else
		{
			fXPos += moveSpeed;
		}
	}

	if (fXPos < -iHitBoxX)
	{
		minionState = -1;
	}
}

void PlayerDart::minionPhysics()
{
	if (!bDestroy)
	{
		Minion::minionPhysics();
	}
}

void PlayerDart::collisionWithPlayer(bool TOP) {}

void PlayerDart::collisionWithAnotherUnit()
{
	bDestroy = true;
	collisionOnlyWithPlayer = true;
}

void PlayerDart::collisionEffect()
{
}

void PlayerDart::setMinionState(int minionState)
{
}