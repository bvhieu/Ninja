#pragma once

#ifndef PLAYER_H
#define PLAYER_H

#include "Sprite.h"
#include "Vector2.h"
#include <vector>
#include <string>

class Player
{
private:
	std::vector<Sprite *> sNinja;
	int iSpriteID;
	unsigned int iMoveAnimationTime;

	float fXPos, fYPos;
	float currentYPos;
	int iNumOfLives;

	unsigned int iScore, maxScore;

	int iFrameID;

	// ----- MOVE

	bool moveDirection; // true = LEFT, false = RIGHT
	bool bMove;
	bool changeMoveDirection;
	bool newMoveDirection;

	static const int maxMove = 4;
	int currentMaxMove;
	int moveSpeed;
	unsigned int iTimePassed;
	// ----- JUMP

	int jumpState;

	float startJumpSpeed;
	float currentJumpSpeed;
	float jumpDistance;
	float currentJumpDistance;

	float currentFallingSpeed;

	// ----- JUMP
	int nextFallFrameID;

	const static int iSmallX = 40, iSmallY = 60;

	int nextDartFrameID;

	// ----- Method
	void movePlayer();

	bool checkCollisionBot(int nX, int nY);
	bool checkCollisionCenter(int nX, int nY);

	Vector2 *getBlockLB(float nX, float nY);
	Vector2 *getBlockRB(float nX, float nY);

	Vector2 *getBlockLC(float nX, float nY);
	Vector2 *getBlockRC(float nX, float nY);

	Vector2 *getBlockLT(float nX, float nY);
	Vector2 *getBlockRT(float nX, float nY);

public:
	Player(SDL_Renderer *renderer, float fXPos, float fYPos);
	~Player(void);

	void Draw(SDL_Renderer *renderer);
	void Update();

	void playerPhysics();

	void updateXPos(int iN);
	void updateYPos(int iN);

	// ----- MOVE
	void moveAnimation();

	void startMove();

	void fightDart();
	void resetDart();
	void createDart();

	void resetMove();
	void stopMove();
	void setMoveDirection(bool moveDirection);
	bool getChangeMoveDirection();
	void setChangeMoveDirection();



	// ----- JUMP
	void jump();
	void startJump(int iH);
	void resetJump();
	// ----- JUMP

	void setNinjaSpriteID(int iID);
	int getNinjaSpriteID();

	int getHitBoxX();
	int getHitBoxY();

	int getXPos();
	void setXPos(float fXPos);
	int getYPos();
	void setYPos(float fYPos);

	int getNumOfLives();
	void setNumOfLives(int iNumOfLives);

	int getMoveSpeed();
	bool getMove();
	bool getMoveDirection();
	void setNextFallFrameID(int nextFallFrameID);
	void setCurrentJumpSpeed(float currentJumpSpeed);
	void setMoveSpeed(int moveSpeed);

	int getJumpState();

	Sprite *getNinjaSprite();

	unsigned int getMaxScore();
	unsigned int getScore();
	void setScore(unsigned int iScore);
};

#endif