#include "Event.h"
#include "Core.h"

Event::Event(void){ 
	this->iDelay = 0;
	this->endGame = false;
	this->iTime = 0;
	this->bState = true;
	this->stepID = 0;
}

Event::~Event(void){ }

void Event::Draw(SDL_Renderer *renderer){ }

void Event::Animation(){
	Normal();
}

void Event::Normal(){
	if (bState)
	{
		if (vOLDDir.size() > stepID)
		{
			if (vOLDLength[stepID] > 0)
			{
				switch (vOLDDir[stepID])
				{
				case eDEATHNOTHING:
					vOLDLength[stepID] -= iSpeed;
					Core::getMap()->getPlayer()->setNinjaSpriteID(0);
					break;
				case eDEATHTOP: // DEATH TOP
					Core::getMap()->getPlayer()->setYPos((float)Core::getMap()->getPlayer()->getYPos() - iSpeed);
					vOLDLength[stepID] -= iSpeed;
					Core::getMap()->getPlayer()->setNinjaSpriteID(0);
					break;
				case eDEATHBOT: // DEATH BOT
					Core::getMap()->getPlayer()->setYPos((float)Core::getMap()->getPlayer()->getYPos() + iSpeed);
					vOLDLength[stepID] -= iSpeed;
					Core::getMap()->getPlayer()->setNinjaSpriteID(0);
					break;
				case eNOTHING: // NOTHING YAY
					vOLDLength[stepID] -= 1;
					break;
				case eLOADINGMENU:
					vOLDLength[stepID] -= 1;

					if (vOLDLength[stepID] < 2)
					{
						Core::getMap()->setInEvent(false);
						inEvent = false;
						Core::getMap()->getPlayer()->stopMove();

						CommonFunctions::getMenuManager()->getLoadingMenu()->loadingType = true;
						CommonFunctions::getMenuManager()->getLoadingMenu()->updateTime();
						CommonFunctions::getMenuManager()->setViewID(CommonFunctions::getMenuManager()->eGameLoading);
					}
					break;
				case eGAMEOVER:
					vOLDLength[stepID] -= 1;

					if (vOLDLength[stepID] < 2)
					{
						Core::getMap()->setInEvent(false);
						inEvent = false;
						Core::getMap()->getPlayer()->stopMove();
						CommonFunctions::getMenuManager()->getLoadingMenu()->loadingType = false;
						CommonFunctions::getMenuManager()->getLoadingMenu()->updateTime();
						CommonFunctions::getMenuManager()->setViewID(CommonFunctions::getMenuManager()->eGameLoading);
						CommonFunctions::getMusic()->PlayChunk(CommonFunctions::getMusic()->cGAMEOVER);
					}
					break;
				}
			}
			else
			{
				++stepID;
				iTime = SDL_GetTicks();
			}
		}
		else
		{
			if (!endGame)
			{
				if (SDL_GetTicks() >= iTime + iDelay)
				{
					bState = false;
					stepID = 0;
					Core::getMap()->getPlayer()->stopMove();
					if (inEvent)
					{
						CommonFunctions::getMenuManager()->getLoadingMenu()->updateTime();
						CommonFunctions::getMenuManager()->getLoadingMenu()->loadingType = true;
						CommonFunctions::getMenuManager()->setViewID(CommonFunctions::getMenuManager()->eGameLoading);
					}

					CommonFunctions::keySpace = false;
				}
			}
			else
			{
				Core::getMap()->resetGameData();
				CommonFunctions::getMenuManager()->setViewID(CommonFunctions::getMenuManager()->eMainMenu);
				Core::getMap()->setInEvent(false);
				Core::getMap()->getPlayer()->stopMove();
				inEvent = false;
				CommonFunctions::keySpace = false;
				endGame = false;
				stepID = 0;
			}
		}
	}
	else
	{
		if (vNEWDir.size() > stepID)
		{
			if (vNEWLength[stepID] > 0)
			{
				switch (vNEWDir[stepID])
				{
				case eNOTHING: // NOTHING YAY
					vNEWLength[stepID] -= 1;
					break;
				}
			}
			else
			{
				++stepID;
			}
		}
		else
		{
			Core::getMap()->setInEvent(false);
			Core::getMap()->getPlayer()->stopMove();
			inEvent = false;
			CommonFunctions::keySpace = false;
			Core::resetKeys();
		}
	}
}

void Event::end(){ }

void Event::resetData(){
	vNEWDir.clear();
	vNEWLength.clear();
	vOLDDir.clear();
	vOLDLength.clear();

	this->bState = true;
	this->stepID = 0;
	this->inEvent = false;
	this->endGame = false;
}
