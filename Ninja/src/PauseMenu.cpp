#include "PauseMenu.h"
#include "CommonFunctions.h"
#include "Core.h"

PauseMenu::PauseMenu(void) {
	rPause.x = 220;
	rPause.y = 140;
	rPause.w = 360;
	rPause.h = 142;

	this->lMenuOption.push_back(new MenuOption("CONTINUE", 0, 156));
	this->lMenuOption.push_back(new MenuOption("OPTIONS", 0, 180));

	this->lMenuOption.push_back(new MenuOption("QUIT TO MENU", 0, 228));
	this->lMenuOption.push_back(new MenuOption("QUIT GAME", 0, 252));

	this->numOfMenuOptions = lMenuOption.size();
}

PauseMenu::~PauseMenu(void) { }

void PauseMenu::Update() { }

void PauseMenu::Draw(SDL_Renderer* renderer) {
	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
	SDL_SetRenderDrawColor(renderer, 4, 4, 4, 235);
	SDL_RenderFillRect(renderer, &rPause);
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	rPause.x += 1;
	rPause.y += 1;
	rPause.h -= 2;
	rPause.w -= 2;
	SDL_RenderDrawRect(renderer, &rPause);
	rPause.x -= 1;
	rPause.y -= 1;
	rPause.h += 2;
	rPause.w += 2;

	for(unsigned int i = 0; i < lMenuOption.size(); i++) {
		if(i == activeMenuOption) {
			CommonFunctions::getText()->DrawCenterX(renderer, lMenuOption[i]->getText(), lMenuOption[i]->getYPos(), 16, 255, 255, 255);
		} else {
			CommonFunctions::getText()->DrawCenterX(renderer, lMenuOption[i]->getText(), lMenuOption[i]->getYPos(), 16, 90, 90, 90);
		}
	}

	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);
	Core::getMap()->setBackgroundColor(renderer);
}

void PauseMenu::enter() {
	switch(activeMenuOption) {
		case 0:
			CommonFunctions::getMenuManager()->setViewID(CommonFunctions::getMenuManager()->eGame);
			CommonFunctions::getMusic()->PlayMusic();
			break;
		case 1:
			CommonFunctions::getMenuManager()->getOptions()->setEscapeToMainMenu(false);
			CommonFunctions::getMenuManager()->resetActiveOptionID(CommonFunctions::getMenuManager()->eOptions);
			CommonFunctions::getMenuManager()->getOptions()->updateVolumeRect();
			CommonFunctions::getMenuManager()->setViewID(CommonFunctions::getMenuManager()->eOptions);
			break;
		case 2:
			Core::getMap()->resetGameData();
			CommonFunctions::getMenuManager()->setViewID(CommonFunctions::getMenuManager()->eMainMenu);
			break;
		case 3:
			Core::quitGame = true;
			break;
	}
}

void PauseMenu::escape() {
	CommonFunctions::getMenuManager()->setViewID(CommonFunctions::getMenuManager()->eGame);
	CommonFunctions::getMusic()->PauseMusic();
}

void PauseMenu::updateActiveButton(int iDir) {
	Menu::updateActiveButton(iDir);
}