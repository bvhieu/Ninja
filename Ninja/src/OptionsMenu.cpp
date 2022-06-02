#include "OptionsMenu.h"
#include "CommonFunctions.h"
#include "Core.h"

OptionsMenu::OptionsMenu(void) {
	rRect.x = 220;
	rRect.y = 48;
	rRect.w = 403;
	rRect.h = 170;

	this->lMenuOption.push_back(new MenuOption("VOLUME", 240, 65));
	this->lMenuOption.push_back(new MenuOption("LEFT", 240, 89));
	this->lMenuOption.push_back(new MenuOption("DOWN", 240, 113));
	this->lMenuOption.push_back(new MenuOption("RIGHT", 240, 137));
	this->lMenuOption.push_back(new MenuOption("JUMP", 240, 161));

	this->numOfMenuOptions = lMenuOption.size();

	this->inSetKey = this->resetSetKey = false;

	rSetKeyRect.x = 230;
	rSetKeyRect.y = 284;
	rSetKeyRect.w = 369;
	rSetKeyRect.h = 71;

	rVolumeBG.x = 350;
	rVolumeBG.y = 65;
	rVolumeBG.h = 16;
	rVolumeBG.w = 200;

	rVolume.x = 350;
	rVolume.y = 65;
	rVolume.h = 16;
	rVolume.w = 100;

	this->escapeToMainMenu = true;
}

OptionsMenu::~OptionsMenu(void) {

}

void OptionsMenu::Update() {
	if(resetSetKey) {
		inSetKey = false;
		resetSetKey = false;
	}
}

void OptionsMenu::Draw(SDL_Renderer* renderer) {
	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
	SDL_SetRenderDrawColor(renderer, 60, 30, 45, 85);
	SDL_RenderFillRect(renderer, &rRect);
	rRect.x += 1;
	rRect.y += 1;
	rRect.h -= 2;
	rRect.w -= 2;
	SDL_RenderDrawRect(renderer, &rRect);
	rRect.x -= 1;
	rRect.y -= 1;
	rRect.h += 2;
	rRect.w += 2;

	for(unsigned int i = 0; i < lMenuOption.size(); i++) {
		if(i == activeMenuOption) {
			CommonFunctions::getText()->Draw(renderer, lMenuOption[i]->getText(), lMenuOption[i]->getXPos(), lMenuOption[i]->getYPos(), 16, 255, 255, 255);
		} else {
			CommonFunctions::getText()->Draw(renderer, lMenuOption[i]->getText(), lMenuOption[i]->getXPos(), lMenuOption[i]->getYPos(), 16, 90, 90, 90);
		}
	}

	SDL_SetRenderDrawColor(renderer, 4, 4, 4, 245);
	SDL_RenderFillRect(renderer, &rVolumeBG);

	SDL_SetRenderDrawColor(renderer, activeMenuOption == 0 ? 150 : 90, activeMenuOption == 0 ? 150 : 90, activeMenuOption == 0 ? 150 : 90, 255);
	SDL_RenderFillRect(renderer, &rVolume);

	if(activeMenuOption == 0) {
		SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
		SDL_RenderDrawRect(renderer, &rVolumeBG);
	} else {
		SDL_SetRenderDrawColor(renderer, 160, 160, 160, 55);
		SDL_RenderDrawRect(renderer, &rVolumeBG);
	}

	CommonFunctions::getText()->Draw(renderer, CommonFunctions::getKeyString(CommonFunctions::keyIDA), 350, 89, 16, activeMenuOption == 1 ? 255 : 90, activeMenuOption == 1 ? 255 : 90, activeMenuOption == 1 ? 255 : 90);
	CommonFunctions::getText()->Draw(renderer, CommonFunctions::getKeyString(CommonFunctions::keyIDS), 350, 113, 16, activeMenuOption == 2 ? 255 : 90, activeMenuOption == 2 ? 255 : 90, activeMenuOption == 2 ? 255 : 90);
	CommonFunctions::getText()->Draw(renderer, CommonFunctions::getKeyString(CommonFunctions::keyIDD), 350, 137, 16, activeMenuOption == 3 ? 255 : 90, activeMenuOption == 3 ? 255 : 90, activeMenuOption == 3 ? 255 : 90);
	CommonFunctions::getText()->Draw(renderer, CommonFunctions::getKeyString(CommonFunctions::keyIDSpace), 350, 161, 16, activeMenuOption == 4 ? 255 : 90, activeMenuOption == 4 ? 255 : 90, activeMenuOption == 4 ? 255 : 90);

	if(inSetKey) {
		SDL_SetRenderDrawColor(renderer, 20, 20, 20, 245);
		SDL_RenderFillRect(renderer, &rSetKeyRect);
		SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
		rSetKeyRect.x += 1;
		rSetKeyRect.y += 1;
		rSetKeyRect.h -= 2;
		rSetKeyRect.w -= 2;
		SDL_RenderDrawRect(renderer, &rSetKeyRect);
		rSetKeyRect.x -= 1;
		rSetKeyRect.y -= 1;
		rSetKeyRect.h += 2;
		rSetKeyRect.w += 2;

		CommonFunctions::getText()->Draw(renderer, "PRESS KEY FOR " + lMenuOption[activeMenuOption]->getText(), 250, rSetKeyRect.y + 16, 16, 255, 255, 255);
		CommonFunctions::getText()->Draw(renderer, "PRESS ESC TO CANCEL", 250, rSetKeyRect.y + 40, 16, 255, 255, 255);
	}

	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);
	Core::getMap()->setBackgroundColor(renderer);
}

void OptionsMenu::enter() {
	switch(activeMenuOption) {
		case 1: case 2: case 3: case 4:
			inSetKey = true;
			break;
	}
}

void OptionsMenu::escape() {
	if(inSetKey) {
		resetSetKey = true;
	} else {
		if(escapeToMainMenu) {
			Core::getMap()->resetGameData();
			CommonFunctions::getMenuManager()->setViewID(CommonFunctions::getMenuManager()->eMainMenu);
		} else {
			CommonFunctions::getMenuManager()->setViewID(CommonFunctions::getMenuManager()->ePasue);
		}
	}
}

void OptionsMenu::setKey(int keyID) {
	if(inSetKey && keyID != SDLK_KP_ENTER && keyID != SDLK_RETURN && keyID != SDLK_ESCAPE) {
		switch(activeMenuOption) {
			case 1:
				CommonFunctions::keyIDA = keyID;
				if(CommonFunctions::keyIDD == keyID) CommonFunctions::keyIDD = 0;
				if(CommonFunctions::keyIDS == keyID) CommonFunctions::keyIDS = 0;
				if(CommonFunctions::keyIDSpace == keyID) CommonFunctions::keyIDSpace = 0;
				break;
			case 2:
				CommonFunctions::keyIDS = keyID;
				if(CommonFunctions::keyIDA == keyID) CommonFunctions::keyIDA = 0;
				if(CommonFunctions::keyIDD == keyID) CommonFunctions::keyIDD = 0;
				if(CommonFunctions::keyIDSpace == keyID) CommonFunctions::keyIDSpace = 0;
				break;
			case 3:
				CommonFunctions::keyIDD = keyID;
				if(CommonFunctions::keyIDA == keyID) CommonFunctions::keyIDA = 0;
				if(CommonFunctions::keyIDS == keyID) CommonFunctions::keyIDS = 0;
				if(CommonFunctions::keyIDSpace == keyID) CommonFunctions::keyIDSpace = 0;
				break;
			case 4:
				CommonFunctions::keyIDSpace = keyID;
				if(CommonFunctions::keyIDA == keyID) CommonFunctions::keyIDA = 0;
				if(CommonFunctions::keyIDS == keyID) CommonFunctions::keyIDS = 0;
				if(CommonFunctions::keyIDD == keyID) CommonFunctions::keyIDD = 0;
				break;
		}
		resetSetKey = true;
	} else if(keyID == SDLK_ESCAPE) {
		resetSetKey = true;
	}
}

void OptionsMenu::updateActiveButton(int iDir) {
	if(activeMenuOption == 0 && (iDir == 1 || iDir == 3)) {
		switch(iDir) {
			case 1:
				if(CommonFunctions::getMusic()->getVolume() < 100) {
					CommonFunctions::getMusic()->setVolume(CommonFunctions::getMusic()->getVolume() + 5);
				} else {
					CommonFunctions::getMusic()->setVolume(100);
				}
				break;
			case 3:
				if(CommonFunctions::getMusic()->getVolume() > 0) {
					CommonFunctions::getMusic()->setVolume(CommonFunctions::getMusic()->getVolume() - 5);
				} else {
					CommonFunctions::getMusic()->setVolume(0);
				}
				break;
		}
		updateVolumeRect();
	}
	if(!inSetKey) {
		Menu::updateActiveButton(iDir);
	}
}

void OptionsMenu::updateVolumeRect() {
	rVolume.w = CommonFunctions::getMusic()->getVolume()*2;
}

void OptionsMenu::setEscapeToMainMenu(bool escapeToMainMenu) {
	this->escapeToMainMenu = escapeToMainMenu;
}