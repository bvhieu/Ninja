#pragma once

#ifndef MENUMANAGER_H
#define MENUMANAGER_H

#include "Image.h"
#include "MainMenu.h"
#include "LoadingMenu.h"
#include "MenuGuide.h"
#include "OptionsMenu.h"
#include "PauseMenu.h"

class MenuManager
{
private:
	Image* activeOption;

	MainMenu* oMainMenu;
	LoadingMenu* oLoadingMenu;
	MenuGuide* oMenuGuide;
	OptionsMenu* oOptionsMenu;
	PauseMenu* oPauseMenu;
public:
	MenuManager(void);
	~MenuManager(void);

	enum gameState {
		eMainMenu,
		eGameLoading,
		eGame,
		eGuide,
		eOptions,
		ePasue,
	};

	gameState currentGameState;

	void Update();
	void Draw(SDL_Renderer* renderer);

	void setBackgroundColor(SDL_Renderer* renderer);

	void enter();
	void escape();
	void setKey(int keyID);
	void keyPressed(int iDir);

	void resetActiveOptionID(gameState ID);

	int getViewID();
	void setViewID(gameState viewID);

	Image* getActiveOption();
	void setActiveOption(SDL_Renderer* renderer);

	LoadingMenu* getLoadingMenu();
	MenuGuide* getMenuGuide();

	OptionsMenu* getOptions();
};

#endif