#include "MenuManager.h"
#include "Core.h"

MenuManager::MenuManager(void) {
	this->currentGameState = eMainMenu;

	this->oMainMenu = new MainMenu();
	this->oLoadingMenu = new LoadingMenu();
	this->oMenuGuide = new MenuGuide();
	this->oOptionsMenu = new OptionsMenu();
	this->oPauseMenu = new PauseMenu();
}

MenuManager::~MenuManager(void) {
	delete activeOption;
	delete oMainMenu;
	delete oLoadingMenu;
	delete oMenuGuide;
}

void MenuManager::Update() {
	switch(currentGameState) {
		case eMainMenu:
			oMainMenu->Update();
			break;
		case eGameLoading:
			oLoadingMenu->Update();
			break;
		case eGame:
			Core::getMap()->Update();
			Core::getMap()->UpdateMinionsCollisions();
			break;
		case eOptions:
			oOptionsMenu->Update();
			break;
		case ePasue:
			oPauseMenu->Update();
			break;
	}
}

void MenuManager::Draw(SDL_Renderer* renderer) {
	switch(currentGameState) {
		case eMainMenu:
			oMainMenu->Draw(renderer);
			break;
		case eGameLoading:
			oLoadingMenu->Draw(renderer);
			break;
		case eGame:
			Core::getMap()->Draw(renderer);
			break;
		case eGuide:
			oMenuGuide->Draw(renderer);
			break;
		case eOptions:
			oOptionsMenu->Draw(renderer);
			break;
		case ePasue:
			Core::getMap()->DrawMap(renderer);
			Core::getMap()->DrawMinions(renderer);
			Core::getMap()->getPlayer()->Draw(renderer);
			Core::getMap()->DrawGameLayout(renderer);
			oPauseMenu->Draw(renderer);
			break;
	}
}

void MenuManager::setBackgroundColor(SDL_Renderer* renderer) {
	switch(currentGameState) {
		case eMainMenu:
			Core::getMap()->setBackgroundColor(renderer);
			break;
		case eGameLoading:
			SDL_SetRenderDrawColor(renderer,150, 190, 200, 255);
			break;
		case eGame:
			Core::getMap()->setBackgroundColor(renderer);
			break;
	}
}

void MenuManager::enter() {
	switch(currentGameState) {
		case eMainMenu:
			oMainMenu->enter();
			break;
		case eGame:
		
			break;
		case eOptions:
			oOptionsMenu->enter();
			break;
		case ePasue:
			oPauseMenu->enter();
			break;
	}
}

void MenuManager::escape() {
	switch(currentGameState) {
		case eGame:
			break;
		case eGuide:
			oMenuGuide->escape();
			break;
		case eOptions:
			oOptionsMenu->escape();
			break;
		case ePasue:
			oPauseMenu->escape();
			break;
	}
}

void MenuManager::setKey(int keyID) {
	switch(currentGameState) {
		case eOptions:
			oOptionsMenu->setKey(keyID);
			break;
	}
}

void MenuManager::keyPressed(int iDir) {
	switch(currentGameState) {
		case eMainMenu:
			oMainMenu->updateActiveButton(iDir);
			break;
		case eOptions:
			oOptionsMenu->updateActiveButton(iDir);
			break;
		case ePasue:
			oPauseMenu->updateActiveButton(iDir);
			break;
	}
}

void MenuManager::resetActiveOptionID(gameState ID) {
	switch(ID) {
		case eMainMenu:
			oMainMenu->activeMenuOption = 0;
			break;
		case eOptions:
			oOptionsMenu->activeMenuOption = 0;
			break;
		case ePasue:
			oPauseMenu->activeMenuOption = 0;
			break;
	}
}

int MenuManager::getViewID() {
	return currentGameState;
}

void MenuManager::setViewID(gameState viewID) {
	this->currentGameState = viewID;
}

Image* MenuManager::getActiveOption() {
	return activeOption;
}

void MenuManager::setActiveOption(SDL_Renderer* renderer) {
	activeOption = new Image("active_option", renderer);
}

LoadingMenu* MenuManager::getLoadingMenu() {
	return oLoadingMenu;
}

MenuGuide* MenuManager::getMenuGuide() {
	return oMenuGuide;
}

OptionsMenu* MenuManager::getOptions() {
	return oOptionsMenu;
}