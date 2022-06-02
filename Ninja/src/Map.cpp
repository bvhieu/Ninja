#include "Map.h"
#include "CommonFunctions.h"
#include "math.h"
#include "stdlib.h"
#include "time.h"

Map::Map(void){ }

Map::Map(SDL_Renderer *renderer){
	oPlayer = new Player(renderer, 84, 368);
	this->levelID = 0;
	this->iMapWidth = 0;
	this->iMapHeight = 0;
	this->fXPos = 0;
	this->fYPos = 0;
	this->inEvent = false;
	this->iSpawnPointID = 0;
	this->bMoveMap = true;
	this->iFrameID = 0;
	CommonFunctions::getText()->setFont(renderer, "font");
	oEvent = new Event();
	srand((unsigned)time(NULL));
	loadGameData(renderer);
	loadMap();
}

Map::~Map(void){
	for (std::vector<Block *>::iterator i = vBlock.begin(); i != vBlock.end(); i++)
	{
		delete (*i);
	}

	for (std::vector<Block *>::iterator i = vMinion.begin(); i != vMinion.end(); i++)
	{
		delete (*i);
	}

	delete oEvent;
}

void Map::Update(){
	UpdateMinionBlokcs();
	UpdateMinions();

	if (!inEvent)
	{
		UpdatePlayer();

		++iFrameID;
		if (iFrameID > 32)
		{
			iFrameID = 0;
			if (iMapTime > 0)
			{
				--iMapTime;

				if (iMapTime <= 0)
				{
					playerDeath(true, true);
				}
			}
		}
	}
	else
	{
		oEvent->Animation();
	}
}

void Map::UpdatePlayer(){
	oPlayer->Update();
}

void Map::UpdateMinions(){
	for (int i = 0; i < iMinionListSize; i++)
	{
		for (int j = 0, jSize = lMinion[i].size(); j < jSize; j++)
		{
			if (lMinion[i][j]->updateMinion())
			{
				lMinion[i][j]->Update();
			}
		}
	}
	// ----- UPDATE MINION LIST ID
	for (int i = 0; i < iMinionListSize; i++)
	{
		for (int j = 0, jSize = lMinion[i].size(); j < jSize; j++)
		{
			if (lMinion[i][j]->minionSpawned)
			{
				if (lMinion[i][j]->minionState == -1)
				{
					delete lMinion[i][j];
					lMinion[i].erase(lMinion[i].begin() + j);
					jSize = lMinion[i].size();
					continue;
				}

				if (floor(lMinion[i][j]->fXPos / 160) != i)
				{
					lMinion[(int)floor((int)lMinion[i][j]->fXPos / 160)].push_back(lMinion[i][j]);
					lMinion[i].erase(lMinion[i].begin() + j);
					jSize = lMinion[i].size();
				}
			}
		}
	}
}

void Map::UpdateMinionsCollisions()
{
	// ----- COLLISIONS
	for (int i = 0; i < iMinionListSize; i++)
	{
		for (unsigned int j = 0; j < lMinion[i].size(); j++)
		{
			if (!lMinion[i][j]->collisionOnlyWithPlayer && lMinion[i][j]->deadTime < 0)
			{
				// ----- WITH MINIONS IN SAME LIST
				for (unsigned int k = j + 1; k < lMinion[i].size(); k++)
				{
					if (!lMinion[i][k]->collisionOnlyWithPlayer && lMinion[i][k]->deadTime < 0)
					{
						if (lMinion[i][j]->getXPos() < lMinion[i][k]->getXPos())
						{

						}
						else
						{
							if (lMinion[i][k]->getXPos() + lMinion[i][k]->iHitBoxX >= lMinion[i][j]->getXPos() && lMinion[i][k]->getXPos() + lMinion[i][k]->iHitBoxX <= lMinion[i][j]->getXPos() + lMinion[i][j]->iHitBoxX && ((lMinion[i][j]->getYPos() <= lMinion[i][k]->getYPos() + lMinion[i][k]->iHitBoxY && lMinion[i][j]->getYPos() + lMinion[i][j]->iHitBoxY >= lMinion[i][k]->getYPos() + lMinion[i][k]->iHitBoxY) || (lMinion[i][k]->getYPos() <= lMinion[i][j]->getYPos() + lMinion[i][j]->iHitBoxY && lMinion[i][k]->getYPos() + lMinion[i][k]->iHitBoxY >= lMinion[i][j]->getYPos() + lMinion[i][j]->iHitBoxY)))
							{
								if (lMinion[i][j]->killOtherUnits && lMinion[i][j]->moveSpeed > 0 && lMinion[i][k]->minionSpawned)
								{
									lMinion[i][k]->setMinionState(-2);
								}

								if (lMinion[i][k]->killOtherUnits && lMinion[i][k]->moveSpeed > 0 && lMinion[i][j]->minionSpawned)
								{
									lMinion[i][j]->setMinionState(-2);
								}

								if (lMinion[i][j]->getYPos() - 4 <= lMinion[i][k]->getYPos() + lMinion[i][k]->iHitBoxY && lMinion[i][j]->getYPos() + 4 >= lMinion[i][k]->getYPos() + lMinion[i][k]->iHitBoxY)
								{
									lMinion[i][k]->onAnotherMinion = true;
								}
								else if (lMinion[i][k]->getYPos() - 4 <= lMinion[i][j]->getYPos() + lMinion[i][j]->iHitBoxY && lMinion[i][k]->getYPos() + 4 >= lMinion[i][j]->getYPos() + lMinion[i][j]->iHitBoxY)
								{
									lMinion[i][j]->onAnotherMinion = true;
								}
								else
								{
									lMinion[i][j]->collisionEffect();
									lMinion[i][k]->collisionEffect();
								}
							}
						}
					}
				}
			}
		}
	}

	if (!inEvent)
	{
		// ----- COLLISION WITH PLAYER
		for (int i = getListID(-(int)fXPos + oPlayer->getXPos()) - (getListID(-(int)fXPos + oPlayer->getXPos()) > 0 ? 1 : 0), iSize = i + 2; i < iSize; i++)
		{
			for (unsigned int j = 0, jSize = lMinion[i].size(); j < jSize; j++)
			{
				if (lMinion[i][j]->deadTime < 0)
				{
					if ((oPlayer->getXPos() - fXPos >= lMinion[i][j]->getXPos() && oPlayer->getXPos() - fXPos <= lMinion[i][j]->getXPos() + lMinion[i][j]->iHitBoxX) || (oPlayer->getXPos() - fXPos + oPlayer->getHitBoxX() >= lMinion[i][j]->getXPos() && oPlayer->getXPos() - fXPos + oPlayer->getHitBoxX() <= lMinion[i][j]->getXPos() + lMinion[i][j]->iHitBoxX))
					{
						if (lMinion[i][j]->getYPos() - 2 <= oPlayer->getYPos() + oPlayer->getHitBoxY() && lMinion[i][j]->getYPos() + 16 >= oPlayer->getYPos() + oPlayer->getHitBoxY())
						{
							lMinion[i][j]->collisionWithPlayer(true);
						}
						else if ((lMinion[i][j]->getYPos() <= oPlayer->getYPos() + oPlayer->getHitBoxY() && lMinion[i][j]->getYPos() + lMinion[i][j]->iHitBoxY >= oPlayer->getYPos() + oPlayer->getHitBoxY()) || (lMinion[i][j]->getYPos() <= oPlayer->getYPos() && lMinion[i][j]->getYPos() + lMinion[i][j]->iHitBoxY >= oPlayer->getYPos()))
						{
							lMinion[i][j]->collisionWithPlayer(false);
						}
					}
				}
			}
		}
	}
}

void Map::UpdateMinionBlokcs(){
	vMinion[0]->getSprite()->Update();
	vMinion[2]->getSprite()->Update();
	vMinion[3]->getSprite()->Update();
	vMinion[4]->getSprite()->Update();
	vMinion[5]->getSprite()->Update();
}

void Map::Draw(SDL_Renderer *renderer){
	DrawMap(renderer);
	DrawMinions(renderer);
	oPlayer->Draw(renderer);
	if (inEvent){
		oEvent->Draw(renderer);
	}
	DrawGameLayout(renderer);
}

void Map::DrawMap(SDL_Renderer *renderer){

	for (int i = getStartBlock(), iEnd = getEndBlock(); i < iEnd && i < iMapWidth; i++)
	{
		for (int j = iMapHeight - 1; j >= 0; j--)
		{
			if (lMap[i][j]->getBlockID() != 0)
			{
				vBlock[lMap[i][j]->getBlockID()]->Draw(renderer, 32 * i + (int)fXPos, CommonFunctions::GAME_HEIGHT - 32 * j - 16 - lMap[i][j]->updateYPos());
			}
		}
	}
}

void Map::DrawMinions(SDL_Renderer *renderer){
	for (int i = 0; i < iMinionListSize; i++)
	{
		for (int j = 0, jSize = lMinion[i].size(); j < jSize; j++)
		{
			lMinion[i][j]->Draw(renderer, vMinion[lMinion[i][j]->getBloockID()]->getSprite()->getTexture());
		}
	}
}

void Map::DrawGameLayout(SDL_Renderer *renderer){
	CommonFunctions::getText()->Draw(renderer, "POINT", 54, 16);
	CommonFunctions::getText()->Draw(renderer, std::to_string(oPlayer->getScore()), 54, 40);

	CommonFunctions::getText()->Draw(renderer, "LEVEL", 462, 16);
	CommonFunctions::getText()->Draw(renderer, getLevelName(), 485, 40);

	CommonFunctions::getText()->Draw(renderer, "MAX", 268, 16);
	CommonFunctions::getText()->Draw(renderer, std::to_string(oPlayer->getMaxScore()), 270, 40);

	CommonFunctions::getText()->Draw(renderer, "TIME", 672, 16);
	if (CommonFunctions::getMenuManager()->getViewID() == CommonFunctions::getMenuManager()->eGame)
	{
		if (iMapTime > 100)
		{
			CommonFunctions::getText()->Draw(renderer, std::to_string(iMapTime), 680, 40);
		}
	}
}

void Map::moveMap(int nX, int nY){
	if (fXPos + nX > 0)
	{
		oPlayer->updateXPos((int)(nX - fXPos));
		fXPos = 0;
	}
	else
	{
		this->fXPos += nX;
	}
}

int Map::getStartBlock(){
	return (int)(-fXPos - (-(int)fXPos) % 32) / 32;
}

int Map::getEndBlock(){
	return (int)(-fXPos - (-(int)fXPos) % 32 + CommonFunctions::GAME_WIDTH) / 32 + 2;
}

// COLLISION

Vector2 *Map::getBlockID(int nX, int nY){
	return new Vector2((int)(nX < 0 ? 0 : nX) / 32, (int)(nY > CommonFunctions::GAME_HEIGHT - 16 ? 0 : (CommonFunctions::GAME_HEIGHT - 16 - nY + 32) / 32));
}

int Map::getBlockIDX(int nX){
	return (int)(nX < 0 ? 0 : nX) / 32;
}

int Map::getBlockIDY(int nY){
	return (int)(nY > CommonFunctions::GAME_HEIGHT - 16 ? 0 : (CommonFunctions::GAME_HEIGHT - 16 - nY + 32) / 32);
}

bool Map::checkCollisionLB(int nX, int nY, int nHitBoxY){
	return checkCollision(getBlockID(nX, nY + nHitBoxY));
}

bool Map::checkCollisionLT(int nX, int nY){
	return checkCollision(getBlockID(nX, nY));
}

bool Map::checkCollisionLC(int nX, int nY, int nHitBoxY){
	return checkCollision(getBlockID(nX, nY + nHitBoxY));
}

bool Map::checkCollisionRC(int nX, int nY, int nHitBoxX, int nHitBoxY){
	return checkCollision(getBlockID(nX + nHitBoxX, nY + nHitBoxY));
}

bool Map::checkCollisionRB(int nX, int nY, int nHitBoxX, int nHitBoxY){
	return checkCollision(getBlockID(nX + nHitBoxX, nY + nHitBoxY));
}

bool Map::checkCollisionRT(int nX, int nY, int nHitBoxX){
	return checkCollision(getBlockID(nX + nHitBoxX, nY));
}

bool Map::checkCollision(Vector2 *nV){
	bool output = vBlock[lMap[nV->getX()][nV->getY()]->getBlockID()]->getCollision();
	delete nV;
	return output;
}

void Map::checkCollisionOnTopOfTheBlock(int nX, int nY){
	for (int i = (nX - nX % 5) / 5, iEnd = i + 3; i < iEnd && i < iMinionListSize; i++)
	{
		for (unsigned int j = 0; j < lMinion[i].size(); j++)
		{
			if (!lMinion[i][j]->collisionOnlyWithPlayer && lMinion[i][j]->getMinionState() >= 0 && ((lMinion[i][j]->getXPos() >= nX * 32 && lMinion[i][j]->getXPos() <= nX * 32 + 32) || (lMinion[i][j]->getXPos() + lMinion[i][j]->iHitBoxX >= nX * 32 && lMinion[i][j]->getXPos() + lMinion[i][j]->iHitBoxX <= nX * 32 + 32)))
			{
				if (lMinion[i][j]->getYPos() + lMinion[i][j]->iHitBoxY >= CommonFunctions::GAME_HEIGHT - 24 - nY * 32 && lMinion[i][j]->getYPos() + lMinion[i][j]->iHitBoxY <= CommonFunctions::GAME_HEIGHT - nY * 32 + 16)
				{
					lMinion[i][j]->moveDirection = !lMinion[i][j]->moveDirection;
					lMinion[i][j]->setMinionState(-2);
				}
			}
		}
	}
}

// COLLISION
// MINION

int Map::getListID(int nX){
	return (int)(nX / 160);
}

void Map::addFrog(int iX, int iY, bool moveDirection){
	lMinion[getListID(iX)].push_back(new Frog(iX, iY, 0, moveDirection));
}

void Map::addDragon(int X, int Y){
	lMinion[getListID(X)].push_back(new Dragon((float)X, (float)Y));
}

void Map::addFire(float fX, float fY, int toYPos){
	lMinion[getListID((int)fX)].push_back(new Fire(fX, fY, toYPos));
}

void Map::addPlayerDart(int X, int Y, bool moveDirection){
	lMinion[getListID(X)].push_back(new PlayerDart(X, Y, moveDirection));
}

/* ---------- LOAD GAME DATA ---------- */

void Map::loadGameData(SDL_Renderer *renderer){
	std::vector<std::string> tSprite;
	std::vector<unsigned int> iDelay;

	//_Transparent
	// 0
	tSprite.push_back("transparent");
	vBlock.push_back(new Block( new Sprite(renderer, tSprite, iDelay), false));
	tSprite.clear();
	//_Ground
	// 1
	tSprite.push_back("ground");
	vBlock.push_back(new Block( new Sprite(renderer, tSprite, iDelay), true));
	tSprite.clear();
	//_Bamboo
	// 2
	tSprite.push_back("bamboo_center_0");
	vBlock.push_back(new Block(new Sprite(renderer, tSprite, iDelay), false));
	tSprite.clear();
	// 3
	tSprite.push_back("bamboo_center_1");
	vBlock.push_back(new Block( new Sprite(renderer, tSprite, iDelay), false));
	tSprite.clear();
	// 4
	tSprite.push_back("bamboo_left");
	vBlock.push_back(new Block( new Sprite(renderer, tSprite, iDelay), false));
	tSprite.clear();
	// 5
	tSprite.push_back("bamboo_right");
	vBlock.push_back(new Block( new Sprite(renderer, tSprite, iDelay), false));
	tSprite.clear();
	// 6
	tSprite.push_back("bamboo_top");
	vBlock.push_back(new Block( new Sprite(renderer, tSprite, iDelay), false));
	tSprite.clear();
	//_Block
	// 7
	tSprite.push_back("block");
	vBlock.push_back(new Block( new Sprite(renderer, tSprite, iDelay), true));
	tSprite.clear();
	//_Grass
	// 8
	tSprite.push_back("grass_left");
	vBlock.push_back(new Block( new Sprite(renderer, tSprite, iDelay), false));
	tSprite.clear();
	// 9
	tSprite.push_back("grass_center");
	vBlock.push_back(new Block(new Sprite(renderer, tSprite, iDelay), false));
	tSprite.clear();
	// 10
	tSprite.push_back("grass_right");
	vBlock.push_back(new Block( new Sprite(renderer, tSprite, iDelay), false));
	tSprite.clear();
	//_Cloud
	// 11
	tSprite.push_back("cloud_left_bot");
	vBlock.push_back(new Block( new Sprite(renderer, tSprite, iDelay), false));
	tSprite.clear();
	// 12
	tSprite.push_back("cloud_left_top");
	vBlock.push_back(new Block(new Sprite(renderer, tSprite, iDelay), false));
	tSprite.clear();
	// 13
	tSprite.push_back("cloud_center_bot");
	vBlock.push_back(new Block( new Sprite(renderer, tSprite, iDelay), false));
	tSprite.clear();
	// 14
	tSprite.push_back("cloud_center_top");
	vBlock.push_back(new Block( new Sprite(renderer, tSprite, iDelay), false));
	tSprite.clear();
	// 15
	tSprite.push_back("cloud_right_bot");
	vBlock.push_back(new Block( new Sprite(renderer, tSprite, iDelay), false));
	tSprite.clear();
	// 16
	tSprite.push_back("cloud_right_top");
	vBlock.push_back(new Block( new Sprite(renderer, tSprite, iDelay), false));
	tSprite.clear();
	//_Next Level
	// 17
	tSprite.push_back("nextLevel");
	vBlock.push_back(new Block( new Sprite(renderer, tSprite, iDelay), false));
	tSprite.clear();

	// --------------- MINION ---------------

	// ----- 0 -----
	tSprite.push_back("frog_0");
	iDelay.push_back(200);
	tSprite.push_back("frog_1");
	iDelay.push_back(200);
	vMinion.push_back(new Block( new Sprite(renderer, tSprite, iDelay), true));
	tSprite.clear();
	iDelay.clear();
	// ----- 1 -----
	tSprite.push_back("frog_death");
	vMinion.push_back(new Block( new Sprite(renderer, tSprite, iDelay), true));
	tSprite.clear();
	iDelay.clear();

	// ----- 2 -----
	tSprite.push_back("dragon0");
	iDelay.push_back(285);
	tSprite.push_back("dragon1");
	iDelay.push_back(285);
	vMinion.push_back(new Block( new Sprite(renderer, tSprite, iDelay), true));
	tSprite.clear();
	iDelay.clear();
	// ----- 3 -----
	tSprite.push_back("dragon2");
	iDelay.push_back(285);
	tSprite.push_back("dragon3");
	iDelay.push_back(285);
	vMinion.push_back(new Block( new Sprite(renderer, tSprite, iDelay), true));
	tSprite.clear();
	iDelay.clear();
	// ----- 4 -----
	tSprite.push_back("fire_0");
	iDelay.push_back(35);
	tSprite.push_back("fire_1");
	iDelay.push_back(35);
	vMinion.push_back(new Block( new Sprite(renderer, tSprite, iDelay), true));
	tSprite.clear();
	iDelay.clear();
	//_Dart
	// 5
	tSprite.push_back("dart_0");
	iDelay.push_back(75);
	tSprite.push_back("dart_1");
	iDelay.push_back(75);
	tSprite.push_back("dart_2");
	iDelay.push_back(75);
	tSprite.push_back("dart_3");
	iDelay.push_back(75);
	vMinion.push_back(new Block( new Sprite(renderer, tSprite, iDelay), true));
	tSprite.clear();
	iDelay.clear();
//_Transparent
	// 6
	tSprite.push_back("transparent");
	vMinion.push_back(new Block( new Sprite(renderer, tSprite, iDelay), true));
	tSprite.clear();
	iDelay.clear();

	iBlockSize = vBlock.size();
	iMinionSize = vMinion.size();
}

void Map::clearMap(){
	for (int i = 0; i < iMapWidth; i++)
	{
		for (int j = 0; j < iMapHeight; j++)
		{
			delete lMap[i][j];
		}
		lMap[i].clear();
	}
	lMap.clear();

	this->iMapWidth = this->iMapHeight = 0;
}

void Map::clearMinions(){
	for (int i = 0; i < iMinionListSize; i++)
	{
		for (int j = 0, jSize = lMinion[i].size(); j < jSize; j++)
		{
			delete lMinion[i][j];
			jSize = lMinion[i].size();
		}
		lMinion[i].clear();
	}
}

void Map::setBackgroundColor(SDL_Renderer *renderer){
	SDL_SetRenderDrawColor(renderer, 174, 215, 255, 1);
}

std::string Map::getLevelName(){
	return "" + std::to_string(1 + getLevelID());
}

void Map::loadMinionsMap_1(){
	clearMinions();

	addDragon(35 * 32, CommonFunctions::GAME_HEIGHT - 16 - 6 * 32);
	addDragon(120 * 32, CommonFunctions::GAME_HEIGHT - 16 - 4 * 32);
	addDragon(135 * 32, CommonFunctions::GAME_HEIGHT - 16 - 6 * 32);
	addDragon(235 * 32, CommonFunctions::GAME_HEIGHT - 16 - 3 * 32);
	addDragon(240 * 32, CommonFunctions::GAME_HEIGHT - 16 - 4 * 32);
	addDragon(250 * 32, CommonFunctions::GAME_HEIGHT - 16 - 3 * 32);

	addFrog(16 * 32, 368, true);
	addFrog(17 * 32 + 18, 368, true);

	addFrog(29 * 32, 368, true);
	addFrog(49 * 32 + 18, 368, true);

	addFrog(62 * 32, 368, true);
	addFrog(64 * 32, 368, true);

	addFrog(73 * 32, 368, true);

	addFrog(76 * 32, 368, true);
	addFrog(77 * 32 + 16, 368, true);

	addFrog(99 * 32, 368, true);
	addFrog(100 * 32 + 16, 368, true);
	addFrog(102 * 32, 368, true);

	addFrog(113 * 32, 368, true);

	addFrog(135 * 32, 368, true);
	addFrog(136 * 32 + 16, 368, true);

	addFrog(163 * 32, 368, true);
	addFrog(164 * 32 + 16, 368, true);

	addFrog(177 * 32, 368, true);
	addFrog(178 * 32 + 16, 368, true);

	addFrog(181 * 32, 368, true);

	addFrog(191 * 32, 368, true);
	addFrog(195 * 32 + 16, 368, true);

	addFrog(205 * 32, 368, true);

	addFrog(227 * 32, 368, true);

	addFrog(249 * 32, 368, true);
	addFrog(250 * 32 + 16, 368, true);
}

void Map::loadMinionsMap_2(){
	clearMinions();

	addDragon( 32, CommonFunctions::GAME_HEIGHT - 16 - 6 * 32);

	addDragon(35 * 32, CommonFunctions::GAME_HEIGHT - 16 - 6 * 32);
	addDragon(135 * 32, CommonFunctions::GAME_HEIGHT - 16 - 3 * 32);

	addFrog(16 * 32, 368, true);
	addFrog(17 * 32 + 8, 368 - 32, true);

	addFrog(29 * 32, 368, true);

	addFrog(62 * 32, 368, true);
	addFrog(64 * 32, 368, true);

	addFrog(73 * 32, 368 - 8 * 32, true);

	addFrog(76 * 32, 368 - 4 * 32, true);
	addFrog(77 * 32 + 16, 368 - 4 * 32, true);

	addFrog(99 * 32, 368, true);
	addFrog(100 * 32 + 16, 368, true);
	addFrog(102 * 32, 368, true);

	addFrog(113 * 32, 368, true);

	addFrog(135 * 32, 368 - 3 * 32, true);
	addFrog(136 * 32 + 16, 368 - 4 * 32, true);
}

void Map::createMap(){
	// ----- MIONION LIST -----
	for (int i = 0; i < iMapWidth; i += 5)
	{
		std::vector<Minion *> temp;
		lMinion.push_back(temp);
	}

	iMinionListSize = lMinion.size();

	// ----- CREATE MAP -----
	for (int i = 0; i < iMapWidth; i++)
	{
		std::vector<MapLevel *> temp;
		for (int i = 0; i < iMapHeight; i++)
		{
			temp.push_back(new MapLevel(0));
		}

		lMap.push_back(temp);
	}
}

int Map::getNumOfSpawnPoints(){
	return 1;
}

int Map::getSpawnPointXPos(int iID){
	return 84;
}

int Map::getSpawnPointYPos(int iID){
	return CommonFunctions::GAME_HEIGHT - 48 - oPlayer->getHitBoxY();
}

void Map::setSpawnPoint(){
	float X = (float)getSpawnPointXPos(iSpawnPointID);

	if (X > 6 * 32)
	{
		fXPos = -(X - 6 * 32);
		oPlayer->setXPos(6 * 32);
		oPlayer->setYPos((float)getSpawnPointYPos(iSpawnPointID));
	}
	else
	{
		fXPos = 0;
		oPlayer->setXPos(X);
		oPlayer->setYPos((float)getSpawnPointYPos(iSpawnPointID));
	}

	oPlayer->setMoveDirection(true);
}

void Map::resetGameData(){
	this->levelID = getLevelID();
	this->iSpawnPointID = 0;

	oPlayer->setScore(0);

	oPlayer->setNumOfLives(3);

	setSpawnPoint();

	loadMap();
}

void Map::loadMap(){
	
	switch (levelID)
	{
	case 0:
		loadMap_1();
		break;
	case 1:
		loadMap_2();
		break;
	}
}

void Map::loadMap_1(){
	clearMap();

	this->iMapWidth = 260;
	this->iMapHeight = 25;
	this->iMapTime = 300;

	createMap();

	// MINIONS
	loadMinionsMap_1();

	// bamboo

	structBamboo(0, 2, 2);
	structBamboo(16, 2, 1);
	structBamboo(48, 2, 2);
	structBamboo(64, 2, 1);
	structBamboo(96, 2, 2);
	structBamboo(112, 2, 1);
	structBamboo(144, 2, 2);
	structBamboo(160, 2, 1);
	structBamboo(184, 2, 2);
	structBamboo(208, 2, 1);
	structBamboo(240, 2, 2);

	// Clouds

	structCloud(8, 10, 1);
	structCloud(19, 11, 1);
	structCloud(27, 10, 3);
	structCloud(36, 11, 2);
	structCloud(56, 10, 1);
	structCloud(67, 11, 1);
	structCloud(75, 10, 3);
	structCloud(84, 11, 2);
	structCloud(104, 10, 1);
	structCloud(115, 11, 1);
	structCloud(123, 10, 3);
	structCloud(132, 11, 2);
	structCloud(152, 10, 1);
	structCloud(163, 11, 1);
	structCloud(171, 10, 3);
	structCloud(180, 11, 2);
	structCloud(200, 10, 1);
	structCloud(211, 11, 1);
	structCloud(219, 10, 3);
	structCloud(228, 11, 2);
	structCloud(248, 10, 1);

	// Grass

	structGrass(11, 2, 3);
	structGrass(29, 2, 1);
	structGrass(41, 2, 2);
	structGrass(59, 2, 3);
	structGrass(71, 2, 1);
	structGrass(89, 2, 2);
	structGrass(107, 2, 3);
	structGrass(119, 2, 1);
	structGrass(137, 2, 2);
	structGrass(157, 2, 1);
	structGrass(167, 2, 1);
	structGrass(205, 2, 1);
	structGrass(215, 2, 1);

	// GND

	structGND(0, 0, 19, 2);
	structGND(21, 0, 2, 1);
	structGND(25, 0, 69, 2);
	structGND(71, 0, 15, 2);
	structGND(89, 0, 64, 2);
	structGND(155, 0, 20, 2);
	structGND(178, 0, 15, 2);
	structGND(196, 0, 2, 1);
	structGND(200, 0, 25, 2);
	structGND(230, 0, 30, 2);

	// Blocks

	struckBlock(10, 4, 4, 1);
	struckBlock(15, 7, 2, 1);
	struckBlock(20, 6, 3, 1);
	struckBlock(25, 2, 1, 1);
	struckBlock(24, 5, 1, 1);
	struckBlock(27, 3, 2, 2);
	struckBlock(29, 4, 4, 1);
	struckBlock(36, 7, 5, 1);
	struckBlock(54, 2, 1, 4);
	struckBlock(55, 2, 1, 2);
	struckBlock(56, 2, 1, 1);
	struckBlock(62, 4, 3, 1);
	struckBlock(76, 2, 1, 1);
	struckBlock(77, 2, 1, 3);
	struckBlock(80, 8, 8, 1);
	struckBlock(90, 9, 3, 1);
	struckBlock(94, 5, 1, 1);
	struckBlock(100, 5, 2, 1);
	struckBlock(118, 5, 1, 1);
	struckBlock(121, 9, 3, 1);
	struckBlock(128, 3, 1, 1);
	struckBlock(131, 5, 1, 1);
	struckBlock(134, 7, 2, 1);
	struckBlock(136, 2, 1, 6);
	struckBlock(145, 2, 1, 1);
	struckBlock(146, 2, 1, 2);
	struckBlock(147, 2, 1, 3);
	struckBlock(148, 2, 1, 4);
	struckBlock(149, 2, 1, 3);
	struckBlock(150, 2, 1, 2);
	struckBlock(151, 2, 1, 1);
	struckBlock(168, 5, 5, 1);
	struckBlock(180, 5, 1, 1);
	struckBlock(184, 7, 1, 1);
	struckBlock(189, 2, 1, 9);
	struckBlock(222, 4, 2, 1);
	struckBlock(230, 4, 2, 1);
	struckBlock(238, 4, 2, 1);
	struckBlock(246, 4, 1, 7);

	struckBlock(257, 2, 1, 12);

	structNextLevel(252, 3);

	structGND(240, 0, 17, 2);
}

void Map::loadMap_2()
{
	clearMap();

	this->iMapWidth = 260;
	this->iMapHeight = 25;
	this->iMapTime = 300;

	createMap();

	// MINIONS

	loadMinionsMap_2();

	// bamboo

	structBamboo(0, 2, 2);
	structBamboo(16, 2, 1);
	structBamboo(48, 2, 2);
	structBamboo(64, 2, 1);
	structBamboo(96, 2, 2);
	structBamboo(112, 2, 1);
	structBamboo(144, 2, 2);
	structBamboo(160, 2, 1);
	structBamboo(192, 2, 2);
	structBamboo(208, 2, 1);
	structBamboo(240, 2, 2);

	// Clouds

	structCloud(8, 10, 1);
	structCloud(19, 11, 1);
	structCloud(27, 10, 3);
	structCloud(36, 11, 2);
	structCloud(56, 10, 1);
	structCloud(67, 11, 1);
	structCloud(75, 10, 3);
	structCloud(84, 11, 2);
	structCloud(104, 10, 1);
	structCloud(115, 11, 1);
	structCloud(123, 10, 3);
	structCloud(132, 11, 2);
	structCloud(152, 10, 1);
	structCloud(163, 11, 1);
	structCloud(171, 10, 3);
	structCloud(180, 11, 2);
	structCloud(200, 10, 1);
	structCloud(211, 11, 1);
	structCloud(219, 10, 3);
	structCloud(228, 11, 2);
	structCloud(248, 10, 1);

	// Grass

	structGrass(11, 2, 3);
	structGrass(23, 2, 1);
	structGrass(41, 2, 2);
	structGrass(59, 2, 3);
	structGrass(71, 2, 1);
	structGrass(89, 2, 2);
	structGrass(107, 2, 3);
	structGrass(119, 2, 1);
	structGrass(137, 2, 2);
	structGrass(157, 2, 1);
	structGrass(167, 2, 1);
	structGrass(205, 2, 1);
	structGrass(215, 2, 1);

	// GND

	structGND(0, 0, 69, 2);
	structGND(71, 0, 15, 2);
	structGND(89, 0, 64, 2);
	structGND(155, 0, 85, 2);

	// Blocks

	struckBlock(10, 4, 4, 1);
	struckBlock(15, 7, 2, 1);
	struckBlock(20, 6, 3, 1);
	struckBlock(24, 5, 1, 1);
	struckBlock(27, 3, 2, 2);
	struckBlock(29, 4, 4, 1);
	struckBlock(36, 7, 5, 1);
	struckBlock(54, 2, 1, 4);
	struckBlock(55, 2, 1, 2);
	struckBlock(56, 2, 1, 1);
	struckBlock(62, 4, 3, 1);
	struckBlock(76, 2, 1, 1);
	struckBlock(77, 2, 1, 3);
	struckBlock(80, 8, 8, 1);
	struckBlock(90, 9, 3, 1);
	struckBlock(94, 5, 1, 1);
	struckBlock(100, 5, 2, 1);
	struckBlock(118, 5, 1, 1);
	struckBlock(121, 9, 3, 1);
	struckBlock(128, 3, 1, 1);
	struckBlock(131, 5, 1, 1);
	struckBlock(134, 7, 2, 1);
	struckBlock(136, 2, 1, 6);
	struckBlock(145, 2, 1, 1);
	struckBlock(146, 2, 1, 2);
	struckBlock(147, 2, 1, 3);
	struckBlock(148, 2, 1, 4);
	struckBlock(149, 2, 1, 3);
	struckBlock(150, 2, 1, 2);
	struckBlock(151, 2, 1, 1);
	struckBlock(168, 5, 5, 1);
	struckBlock(180, 5, 1, 1);
	struckBlock(184, 7, 1, 1);
	struckBlock(189, 2, 1, 9);
	struckBlock(222, 4, 2, 1);
	struckBlock(230, 4, 2, 1);
	struckBlock(238, 4, 2, 1);
	struckBlock(246, 4, 1, 7);

	struckBlock(259, 2, 1, 12);

	structNextLevel(255, 3);
	structNextLevel(10, 3);

	structGND(240, 0, 17, 2);
}

void Map::playerDeath(bool animation, bool instantDeath)
{
	if (instantDeath)
	{
		inEvent = true;

		oEvent->resetData();
		oPlayer->resetJump();
		oPlayer->stopMove();

		oEvent->iDelay = 150;
		if (animation)
		{
			oEvent->iSpeed = 4;

			oPlayer->setYPos(oPlayer->getYPos() + 4.0f);

			oEvent->vOLDDir.push_back(oEvent->eDEATHNOTHING);
			oEvent->vOLDLength.push_back(30);

			oEvent->vOLDDir.push_back(oEvent->eDEATHTOP);
			oEvent->vOLDLength.push_back(64);

			oEvent->vOLDDir.push_back(oEvent->eDEATHBOT);
			oEvent->vOLDLength.push_back(CommonFunctions::GAME_HEIGHT - oPlayer->getYPos() + 128);
		}
		else
		{
			oEvent->iSpeed = 4;

			oEvent->vOLDDir.push_back(oEvent->eDEATHTOP);
			oEvent->vOLDLength.push_back(1);
		}

		oEvent->vOLDDir.push_back(oEvent->eNOTHING);
		oEvent->vOLDLength.push_back(64);

		if (oPlayer->getNumOfLives() > 1)
		{
			oEvent->vOLDDir.push_back(oEvent->eLOADINGMENU);
			oEvent->vOLDLength.push_back(90);

			oPlayer->setNumOfLives(oPlayer->getNumOfLives() - 1);

			CommonFunctions::getMusic()->StopMusic();
			CommonFunctions::getMusic()->PlayChunk(CommonFunctions::getMusic()->cDEATH);
		}
		else
		{
			oEvent->vOLDDir.push_back(oEvent->eGAMEOVER);
			oEvent->vOLDLength.push_back(90);

			oPlayer->setNumOfLives(oPlayer->getNumOfLives() - 1);

			CommonFunctions::getMusic()->StopMusic();
			CommonFunctions::getMusic()->PlayChunk(CommonFunctions::getMusic()->cDEATH);
		}
	}
}

void Map::structBamboo(int X, int Y, int iSize)
{
	// ----- LEFT & RIGHT
	for (int i = 0; i < iSize; i++)
	{
		lMap[X + i][Y + i]->setBlockID(4);
		lMap[X + iSize + 1 + i][Y + iSize - 1 - i]->setBlockID(5);
	}

	// ----- CENTER LEFT & RIGHT
	for (int i = 0, k = 1; i < iSize - 1; i++)
	{
		for (int j = 0; j < k; j++)
		{
			lMap[X + 1 + i][Y + j]->setBlockID((i % 2 == 0 ? 2 : 3));
			lMap[X + iSize * 2 - 1 - i][Y + j]->setBlockID((i % 2 == 0 ? 2 : 3));
		}
		++k;
	}

	// ----- CENTER
	for (int i = 0; i < iSize; i++)
	{
		lMap[X + iSize][Y + i]->setBlockID((i % 2 == 0 && iSize != 1 ? 3 : 2));
	}

	// ----- TOP
	lMap[X + iSize][Y + iSize]->setBlockID(6);
}

void Map::structGrass(int X, int Y, int iSize)
{
	lMap[X][Y]->setBlockID(8);
	for (int i = 0; i < iSize; i++)
	{
		lMap[X + 1 + i][Y]->setBlockID(9);
	}
	lMap[X + iSize + 1][Y]->setBlockID(10);
}

void Map::structCloud(int X, int Y, int iSize)
{
	// ----- LEFT
	lMap[X][Y]->setBlockID(11);
	lMap[X][Y + 1]->setBlockID(12);

	for (int i = 0; i < iSize; i++)
	{
		lMap[X + 1 + i][Y]->setBlockID(13);
		lMap[X + 1 + i][Y + 1]->setBlockID(14);
	}

	lMap[X + iSize + 1][Y]->setBlockID(15);
	lMap[X + iSize + 1][Y + 1]->setBlockID(16);
}

void Map::structGND(int X, int Y, int iWidth, int iHeight)
{
	for (int i = 0; i < iWidth; i++)
	{
		for (int j = 0; j < iHeight; j++)
		{
			lMap[X + i][Y + j]->setBlockID(1);
		}
	}
}

void Map::struckBlock(int X, int Y, int iWidth, int iHeight)
{
	for (int i = 0; i < iWidth; i++)
	{
		for (int j = 0; j < iHeight; j++)
		{
			lMap[X + i][Y + j]->setBlockID(7);
		}
	}
}

void Map::structNextLevel(int X, int Y)
{
	lMap[X][Y]->setBlockID(17);
}

void Map::setBlockID(int X, int Y, int iBlockID)
{
	if (X >= 0 && X < iMapWidth)
	{
		lMap[X][Y]->setBlockID(iBlockID);
	}
}

Player *Map::getPlayer()
{
	return oPlayer;
}

int Map::getLevelID()
{
	return levelID;
}

float Map::getXPos()
{
	return fXPos;
}

void Map::setXPos(float iXPos)
{
	this->fXPos = iXPos;
}

float Map::getYPos()
{
	return fYPos;
}

void Map::setYPos(float iYPos)
{
	this->fYPos = iYPos;
}

void Map::setSpawnPointID(int iSpawnPointID)
{
	this->iSpawnPointID = iSpawnPointID;
}

int Map::getMapTime()
{
	return iMapTime;
}

void Map::setMapTime(int iMapTime)
{
	this->iMapTime = iMapTime;
}

int Map::getMapWidth()
{
	return iMapWidth;
}

bool Map::getMoveMap()
{
	return bMoveMap;
}

void Map::setMoveMap(bool bMoveMap)
{
	this->bMoveMap = bMoveMap;
}

Event *Map::getEvent()
{
	return oEvent;
}

bool Map::getInEvent()
{
	return inEvent;
}

void Map::setInEvent(bool inEvent)
{
	this->inEvent = inEvent;
}

Block *Map::getBlock(int iID)
{
	return vBlock[iID];
}

Block *Map::getMinionBlock(int iID)
{
	return vMinion[iID];
}

MapLevel *Map::getMapBlock(int iX, int iY)
{
	return lMap[iX][iY];
}
