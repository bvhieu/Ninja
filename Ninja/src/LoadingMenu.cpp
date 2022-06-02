#include "LoadingMenu.h"
#include "CommonFunctions.h"
#include "Core.h"

LoadingMenu::LoadingMenu(void) {
	this->iTime = 0;
	this->loadingType = true;
}

LoadingMenu::~LoadingMenu(void) { }

void LoadingMenu::Update() {
	if(SDL_GetTicks() >= iTime + 2500 + (loadingType ? 0 : 2750)) {
		CommonFunctions::getMusic()->PlayMusic();
		if(loadingType) {
			if(!Core::getMap()->getInEvent()) {
				Core::getMap()->setSpawnPoint();
				Core::getMap()->loadMap();
			}
			CommonFunctions::getMenuManager()->setViewID(CommonFunctions::getMenuManager()->eGame);
		} else {
			Core::getMap()->resetGameData();
			CommonFunctions::getMenuManager()->setViewID(CommonFunctions::getMenuManager()->eMainMenu);
		}
	} else {
		CommonFunctions::getMusic()->StopMusic();
	}
}

void LoadingMenu::Draw(SDL_Renderer* renderer) {
	if(loadingType) {
		Core::getMap()->DrawGameLayout(renderer);
		Core::getMap()->getPlayer()->getNinjaSprite()->getTexture()->Draw(renderer, 342, 210 - Core::getMap()->getPlayer()->getHitBoxY()/2);
		if(Core::getMap()->getPlayer()->getNumOfLives() > 9) {
			Core::getMap()->getBlock(180)->getSprite()->getTexture()->Draw(renderer, 410, 210);
		}
		CommonFunctions::getText()->Draw(renderer, std::to_string(Core::getMap()->getPlayer()->getNumOfLives()), 432, 208);
	} else {
		Core::getMap()->DrawGameLayout(renderer);
		CommonFunctions::getText()->DrawCenterX(renderer, "GAME OVER", 240, 16);
	}
}

void LoadingMenu::updateTime() {
	this->iTime = SDL_GetTicks();
}