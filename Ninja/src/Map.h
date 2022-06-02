#pragma once

#ifndef MAP_H
#define MAP_H

#include "header.h"
#include "Image.h"
#include "Block.h"
#include "MapLevel.h"
#include "Player.h"
#include "Vector2.h"
#include "Frog.h"
#include "Dragon.h"
#include "Fire.h"
#include "Event.h"
#include "PlayerDart.h"
#include <vector>

class Map
{
private:
	float fXPos, fYPos;

	std::vector<Block *> vBlock;
	int iBlockSize; // Size of vBlock

	std::vector<Block *> vMinion;
	int iMinionSize; // Size of vBlock

	std::vector<std::vector<MapLevel *>> lMap;
	int iMapWidth, iMapHeight;

	int levelID;

	int iSpawnPointID;

	bool bMoveMap;

	int iFrameID;
	int iMapTime;

	bool inEvent;
	Event *oEvent;

	// ----- PLAYER -----
	Player *oPlayer;

	// ----- MINIONS -----
	std::vector<std::vector<Minion *>> lMinion;
	int iMinionListSize;

	int getListID(int nXPos);

	// ---------- Methods ----------

	int getStartBlock();
	int getEndBlock();

	// ----- Load -----
	void loadGameData(SDL_Renderer *renderer);

	void createMap();

	void checkSpawnPoint();
	int getNumOfSpawnPoints();
	int getSpawnPointXPos(int iID);
	int getSpawnPointYPos(int iID);

	void loadMap_1();
	void loadMap_2();

	void loadMinionsMap_1();
	void loadMinionsMap_2();

	void clearMap();
	void clearMinions();

public:
	Map(void);
	Map(SDL_Renderer *renderer);
	~Map(void);

	void Update();

	void UpdatePlayer();
	void UpdateMinions();
	void UpdateMinionsCollisions();
	void UpdateMinionBlokcs();

	void Draw(SDL_Renderer *renderer);
	void DrawMap(SDL_Renderer *renderer);
	void DrawMinions(SDL_Renderer *renderer);
	void DrawGameLayout(SDL_Renderer *renderer);

	void moveMap(int iX, int iY);
	void setSpawnPoint();

	void addFrog(int iX, int iY, bool moveDirection);
	void addDragon(int X, int Y);
	void addFire(float fX, float fY, int toYPos);
	void addPlayerDart(int X, int Y, bool moveDirection);
	void lockMinions();

	// ********** COLLISION
	Vector2 *getBlockID(int nX, int nY);
	int getBlockIDX(int nX);
	int getBlockIDY(int nY);

	// ----- LEFT
	bool checkCollisionLB(int nX, int nY, int nHitBoxY);
	bool checkCollisionLT(int nX, int nY);
	// ----- CENTER
	bool checkCollisionLC(int nX, int nY, int nHitBoxY);
	bool checkCollisionRC(int nX, int nY, int nHitBoxX, int nHitBoxY);
	// ----- RIGHT
	bool checkCollisionRB(int nX, int nY, int nHitBoxX, int nHitBoxY);
	bool checkCollisionRT(int nX, int nY, int nHitBoxX);

	bool checkCollision(Vector2 *nV);

	void checkCollisionOnTopOfTheBlock(int nX, int nY);

	void playerDeath(bool animation, bool instantDeath);

	// ----- LOAD
	void resetGameData();
	void loadMap();
	void setBackgroundColor(SDL_Renderer *renderer);
	std::string getLevelName();

	void structBamboo(int X, int Y, int iSze);
	void structGrass(int X, int Y, int iSize);
	void structCloud(int X, int Y, int iSize);
	void structGND(int X, int Y, int iWidth, int iHeight);
	void struckBlock(int X, int Y, int iWidth, int iHeight);
	void structNextLevel(int X, int Y);
	void setBlockID(int X, int Y, int iBlockID);

	// ----- get & set -----
	int getLevelID();
	float getXPos();
	void setXPos(float iYPos);
	float getYPos();
	void setYPos(float iYPos);

	int getMapTime();
	void setMapTime(int iMapTime);

	void setSpawnPointID(int iSpawnPointID);

	int getMapWidth();

	Block *getBlock(int iID);
	Block *getMinionBlock(int iID);
	MapLevel *getMapBlock(int iX, int iY);

	Player *getPlayer();

	bool getMoveMap();
	void setMoveMap(bool bMoveMap);

	Event *getEvent();
	bool getInEvent();
	void setInEvent(bool inEvent);
};

#endif