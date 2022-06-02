#include "Player.h"
#include "Core.h"
#include "stdlib.h"
#include "time.h"


Player::Player(SDL_Renderer *renderer, float fXPos, float fYPos)
{
	this->fXPos = fXPos;
	this->fYPos = fYPos;
	this->iNumOfLives = 3;

	this->iSpriteID = 1;

	this->maxScore = 0; ///////

	this->iScore = 0;
	this->iFrameID = 0;

	this->nextFallFrameID = 0;

	this->moveDirection = true;
	this->currentMaxMove = 4;
	this->moveSpeed = 0;
	this->bMove = false;
	this->changeMoveDirection = false;

	this->iTimePassed = SDL_GetTicks();

	this->jumpState = 0;
	this->startJumpSpeed = 7.00f;
	this->currentFallingSpeed = 3.0f;

	this->iMoveAnimationTime = 0;

	this->nextDartFrameID = 8;

	// ----- LOAD SPRITE
	std::vector<std::string> tempS;
	std::vector<unsigned int> tempI;

	srand((unsigned)time(NULL));

	// ----- 0
	tempS.push_back("ninja/ninja_death");
	tempI.push_back(0);
	sNinja.push_back(new Sprite(renderer, tempS, tempI));
	tempS.clear();
	// ----- 1
	tempS.push_back("ninja/ninja");
	sNinja.push_back(new Sprite(renderer, tempS, tempI));
	tempS.clear();
	// ----- 2
	tempS.push_back("ninja/ninja_move0");
	sNinja.push_back(new Sprite(renderer, tempS, tempI));
	tempS.clear();
	// ----- 3
	tempS.push_back("ninja/ninja_move1");
	sNinja.push_back(new Sprite(renderer, tempS, tempI));
	tempS.clear();
	// ----- 4
	tempS.push_back("ninja/ninja_move2");
	sNinja.push_back(new Sprite(renderer, tempS, tempI));
	tempS.clear();
	// ----- 5
	tempS.push_back("ninja/ninja_move3");
	sNinja.push_back(new Sprite(renderer, tempS, tempI));
	tempS.clear();
	// ----- 6
	tempS.push_back("ninja/ninja_jump");
	sNinja.push_back(new Sprite(renderer, tempS, tempI));
	tempS.clear();
}

Player::~Player(void)
{
	for (std::vector<Sprite *>::iterator i = sNinja.begin(); i != sNinja.end(); i++)
	{
		delete (*i);
	}
}

void Player::Update()
{
	playerPhysics();
	movePlayer();

	if (iFrameID > 0)
	{
		--iFrameID;
	}

	if(nextDartFrameID > 0) {
		--nextDartFrameID;
	}

}

void Player::playerPhysics()
{
	if (jumpState == 1)
	{
		updateYPos(-(int)currentJumpSpeed);
		currentJumpDistance += (int)currentJumpSpeed;

		currentJumpSpeed *= (currentJumpDistance / jumpDistance > 0.75f ? 0.972f : 0.986f);

		if (currentJumpSpeed < 2.5f)
		{
			currentJumpSpeed = 2.5f;
		}

		if (!CommonFunctions::keySpace && currentJumpDistance > 64)
		{
			jumpDistance = 16;
			currentJumpDistance = 0;
			currentJumpSpeed = 2.5f;
		}

		if (jumpDistance <= currentJumpDistance)
		{
			jumpState = 2;
		}
	}
	else
	{

		if (!Core::getMap()->checkCollisionLB((int)(fXPos - Core::getMap()->getXPos() + 2), (int)fYPos + 2, getHitBoxY()) &&
			!Core::getMap()->checkCollisionRB((int)(fXPos - Core::getMap()->getXPos() - 2), (int)fYPos + 2, getHitBoxX(), getHitBoxY()))
		{

			if (nextFallFrameID > 0)
			{
				--nextFallFrameID;
			}
			else
			{
				currentFallingSpeed *= 1.05f;

				if (currentFallingSpeed > startJumpSpeed)
				{
					currentFallingSpeed = startJumpSpeed;
				}

				updateYPos((int)currentFallingSpeed);
			}
			jumpState = 2;
		}
		else if (jumpState == 2)
		{
			resetJump();
		}
		else
		{
			checkCollisionBot(0, 0);
		}
	}
}

void Player::movePlayer()
{
	if (bMove && !changeMoveDirection)
	{
		if (moveSpeed > currentMaxMove)
		{
			--moveSpeed;
		}
		else if (SDL_GetTicks() - (100 + 35 * moveSpeed) >= iTimePassed && moveSpeed < currentMaxMove)
		{
			++moveSpeed;
			iTimePassed = SDL_GetTicks();
		}
		else if (moveSpeed == 0)
		{
			moveSpeed = 1;
		}
	}
	else
	{
		if (SDL_GetTicks() - (50 + 15 * (currentMaxMove - moveSpeed)) > iTimePassed && moveSpeed != 0)
		{
			--moveSpeed;
			iTimePassed = SDL_GetTicks();
		}

		if (changeMoveDirection && moveSpeed <= 1)
		{
			moveDirection = newMoveDirection;
			changeMoveDirection = false;
			bMove = true;
		}
	}

	if (moveSpeed > 0)
	{
		if (moveDirection)
		{
			updateXPos(moveSpeed);
		}
		else
		{
			updateXPos(-moveSpeed);
		}

		// ----- SPRITE ANIMATION
		if (!changeMoveDirection && jumpState == 0 && bMove)
		{
			moveAnimation();
		}
		// ----- SPRITE ANIMATION
	}
	else if (jumpState == 0)
	{
		setNinjaSpriteID(1);
		updateXPos(0);
	}
	else
	{
		updateXPos(0);
	}
}

void Player::moveAnimation()
{
	if (SDL_GetTicks() - 65 + moveSpeed * 5 > iMoveAnimationTime)
	{
		iMoveAnimationTime = SDL_GetTicks();
		if (iSpriteID < 5)
		{
			setNinjaSpriteID(iSpriteID);
			++iSpriteID;
		}
		else
		{
			iSpriteID = 1;
		}
	}
}

void Player::startMove()
{
	iMoveAnimationTime = SDL_GetTicks();
	iTimePassed = SDL_GetTicks();
	moveSpeed = 1;
	bMove = true;
}

void Player::fightDart()
{
	currentMaxMove = maxMove + 2;
	createDart();
}

void Player::resetDart()
{
	currentMaxMove = maxMove;
}

void Player::createDart()
{
		if (currentYPos != fYPos){currentYPos = fYPos;};
		if(nextDartFrameID <= 0) {
			Core::getMap()->addPlayerDart((int)(fXPos - Core::getMap()->getXPos() + (moveDirection ? getHitBoxX() : -32)),currentYPos + 15, !moveDirection);
			nextDartFrameID = 16;
		}
}
void Player::resetMove()
{
	--moveSpeed;
	bMove = false;
}

void Player::stopMove()
{
	moveSpeed = 0;
	bMove = false;
	changeMoveDirection = false;
	setNinjaSpriteID(1);
}


void Player::jump()
{
	if (jumpState == 0)
	{
		startJump(4);
	}
}

void Player::startJump(int iH)
{
	currentJumpSpeed = startJumpSpeed;
	jumpDistance = 32 * iH + 24.0f;
	currentJumpDistance = 0;
	CommonFunctions::getMusic()->PlayChunk(CommonFunctions::getMusic()->cJUMP);
	jumpState = 1;
}

void Player::resetJump()
{
	jumpState = 0;
	jumpDistance = 0;
	currentJumpDistance = 0;
	currentFallingSpeed = 4.0f;
	nextFallFrameID = 0;
}

void Player::updateXPos(int iN)
{
	checkCollisionBot(iN, 0);
	checkCollisionCenter(iN, 0);
	if (iN > 0)
	{
		if (!Core::getMap()->checkCollisionRB((int)(fXPos - Core::getMap()->getXPos() + iN), (int)fYPos - 2, getHitBoxX(), getHitBoxY()) && !Core::getMap()->checkCollisionRT((int)(fXPos - Core::getMap()->getXPos() + iN), (int)fYPos + 2, getHitBoxX()) &&
			(true ))
		{
			if (fXPos >= 416 && Core::getMap()->getMoveMap())
			{
				Core::getMap()->moveMap(-iN, 0);
			}
			else
			{
				fXPos += iN;
			}
		}
		else
		{
			updateXPos(iN - 1);
			if (moveSpeed > 1 && jumpState == 0)
				--moveSpeed;
		}
	}
	else if (iN < 0)
	{
		if (!Core::getMap()->checkCollisionLB((int)(fXPos - Core::getMap()->getXPos() + iN), (int)fYPos - 2, getHitBoxY()) && !Core::getMap()->checkCollisionLT((int)(fXPos - Core::getMap()->getXPos() + iN), (int)fYPos + 2) &&
			(true))
		{
			if (fXPos <= 192 && Core::getMap()->getXPos() && Core::getMap()->getMoveMap())
			{

				Core::getMap()->moveMap(-iN, 0);
			}
			else if (fXPos - Core::getMap()->getXPos() + iN >= 0 && fXPos >= 0)
			{
				fXPos += iN;
			}
			else if (fXPos >= 0)
			{
				updateXPos(iN + 1);
			}
		}
		else
		{
			updateXPos(iN + 1);
			if (moveSpeed > 1 && jumpState == 0)
				--moveSpeed;
		}
	}
}

void Player::updateYPos(int iN)
{
	bool bLEFT, bRIGHT;

	if (iN > 0)
	{
		bLEFT = Core::getMap()->checkCollisionLB((int)(fXPos - Core::getMap()->getXPos() + 2), (int)fYPos + iN, getHitBoxY());
		bRIGHT = Core::getMap()->checkCollisionRB((int)(fXPos - Core::getMap()->getXPos() - 2), (int)fYPos + iN, getHitBoxX(), getHitBoxY());

		if (!bLEFT && !bRIGHT)
		{
			fYPos += iN;
		}
		else
		{
			if (jumpState == 2)
			{
				jumpState = 0;
			}
			updateYPos(iN - 1);
		}
	}
	else if (iN < 0)
	{
		bLEFT = Core::getMap()->checkCollisionLT((int)(fXPos - Core::getMap()->getXPos() + 2), (int)fYPos + iN);
		bRIGHT = Core::getMap()->checkCollisionRT((int)(fXPos - Core::getMap()->getXPos() - 2), (int)fYPos + iN, getHitBoxX());

		if (!bLEFT && !bRIGHT)
		{
			fYPos += iN;
		}
		else
		{
			if (jumpState == 1)
			{
				if (!bLEFT && bRIGHT)
				{
					Vector2 *vRT = getBlockRT(fXPos - Core::getMap()->getXPos(), fYPos + iN);

					if (!Core::getMap()->getBlock(Core::getMap()->getMapBlock(vRT->getX(), vRT->getY())->getBlockID()))
					{
							fYPos += iN;					
					}
					else if ((int)(fXPos + getHitBoxX() - Core::getMap()->getXPos()) % 32 <= 8)
					{
						updateXPos((int)-((int)(fXPos + getHitBoxX() - Core::getMap()->getXPos()) % 32));
					}
					else if (Core::getMap()->getBlock(Core::getMap()->getMapBlock(vRT->getX(), vRT->getY())->getBlockID()))
					{
						
							fYPos += iN;
					}
					else
					{
						jumpState = 2;
					}

					delete vRT;
				}
				else if (bLEFT && !bRIGHT)
				{
					Vector2 *vLT = getBlockLT(fXPos - Core::getMap()->getXPos(), fYPos + iN);
					if (!Core::getMap()->getBlock(Core::getMap()->getMapBlock(vLT->getX(), vLT->getY())->getBlockID()))
					{
							fYPos += iN;
					}
					else if ((int)(fXPos - Core::getMap()->getXPos()) % 32 >= 24)
					{
						updateXPos((int)(32 - (int)(fXPos - Core::getMap()->getXPos()) % 32));
					}
					else if (Core::getMap()->getBlock(Core::getMap()->getMapBlock(vLT->getX(), vLT->getY())->getBlockID()))
					{
							fYPos += iN;
					}
					else
					{
						jumpState = 2;
					}

					delete vLT;
				}
				else
				{
					if ((int)(fXPos + getHitBoxX() - Core::getMap()->getXPos()) % 32 > 32 - (int)(fXPos - Core::getMap()->getXPos()) % 32)
					{
						Vector2 *vRT = getBlockRT(fXPos - Core::getMap()->getXPos(), fYPos + iN);
							jumpState = 2;
						delete vRT;
					}
					else
					{
						Vector2 *vLT = getBlockLT(fXPos - Core::getMap()->getXPos(), fYPos + iN);				
							jumpState = 2;
						delete vLT;
					}
				}
			}

			updateYPos(iN + 1);
		}
	}

	if ((int)fYPos % 2 == 1)
	{
		fYPos += 1;
	}

	if (!Core::getMap()->getInEvent() && fYPos - getHitBoxY() > CommonFunctions::GAME_HEIGHT)
	{
		Core::getMap()->playerDeath(false, true);
		fYPos = -80;
	}
}

bool Player::checkCollisionBot(int nX, int nY)
{
	Vector2 *vLT = getBlockLB(fXPos - Core::getMap()->getXPos() + nX, fYPos + nY);

	delete vLT;

	vLT = getBlockRB(fXPos - Core::getMap()->getXPos() + nX, fYPos + nY);

	delete vLT;
	return true;
}

bool Player::checkCollisionCenter(int nX, int nY)
{
		Vector2 *vLT = getBlockLC(fXPos - Core::getMap()->getXPos() + nX, fYPos + nY);
		delete vLT;
		vLT = getBlockRC(fXPos - Core::getMap()->getXPos() + nX, fYPos + nY);
		delete vLT;
	return true;
}

Vector2 *Player::getBlockLB(float nX, float nY)
{
	return Core::getMap()->getBlockID((int)nX + 1, (int)nY + getHitBoxY() + 2);
}

Vector2 *Player::getBlockRB(float nX, float nY)
{
	return Core::getMap()->getBlockID((int)nX + getHitBoxX() - 1, (int)nY + getHitBoxY() + 2);
}

Vector2 *Player::getBlockLC(float nX, float nY)
{
	return Core::getMap()->getBlockID((int)nX - 1, (int)nY + getHitBoxY() / 2);
}

Vector2 *Player::getBlockRC(float nX, float nY)
{
	return Core::getMap()->getBlockID((int)nX + getHitBoxX() + 1, (int)nY + getHitBoxY() / 2);
}

Vector2 *Player::getBlockLT(float nX, float nY)
{
	return Core::getMap()->getBlockID((int)nX + 1, (int)nY);
}

Vector2 *Player::getBlockRT(float nX, float nY)
{
	return Core::getMap()->getBlockID((int)nX + getHitBoxX() - 1, (int)nY);
}

void Player::Draw(SDL_Renderer *renderer)
{
	sNinja[getNinjaSpriteID()]->getTexture()->Draw(renderer, (int)fXPos, (int)fYPos + 2, !moveDirection);
}

int Player::getNinjaSpriteID()
{
	return iSpriteID;
}

void Player::setNinjaSpriteID(int iID)
{
	this->iSpriteID = iID ;
}

int Player::getHitBoxX()
{
	return iSmallX;
}

int Player::getHitBoxY()
{
	return iSmallY;
}

void Player::setMoveDirection(bool moveDirection)
{
	this->moveDirection = moveDirection;
}

bool Player::getChangeMoveDirection()
{
	return changeMoveDirection;
}

void Player::setChangeMoveDirection()
{
	this->changeMoveDirection = true;
	this->newMoveDirection = !moveDirection;
}

int Player::getXPos()
{
	return (int)fXPos;
}

void Player::setXPos(float fXPos)
{
	this->fXPos = fXPos;
}

int Player::getYPos()
{
	return (int)fYPos;
}

int Player::getNumOfLives()
{
	return iNumOfLives;
}

void Player::setNumOfLives(int iNumOfLives)
{
	this->iNumOfLives = iNumOfLives;
}

int Player::getMoveSpeed()
{
	return moveSpeed;
}

int Player::getJumpState()
{
	return jumpState;
}

bool Player::getMove()
{
	return bMove;
}

bool Player::getMoveDirection()
{
	return moveDirection;
}

void Player::setNextFallFrameID(int nextFallFrameID)
{
	this->nextFallFrameID = nextFallFrameID;
}

void Player::setCurrentJumpSpeed(float currentJumpSpeed)
{
	this->currentJumpSpeed = currentJumpSpeed;
}

void Player::setMoveSpeed(int moveSpeed)
{
	this->moveSpeed = moveSpeed;
}

void Player::setYPos(float fYPos)
{
	this->fYPos = fYPos;
}

unsigned int Player::getMaxScore()
{
	if (iScore > maxScore)
	{
		maxScore = iScore;
	}
	return maxScore;
}

unsigned int Player::getScore()
{
	return iScore;
}

void Player::setScore(unsigned int iScore)
{
	this->iScore = iScore;
}

Sprite *Player::getNinjaSprite()
{
	return sNinja[1];
}