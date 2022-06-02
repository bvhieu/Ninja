#pragma once

#ifndef MENU_H
#define MENU_h

#include "header.h"
#include "MenuOption.h"
#include <vector>

class Menu
{
public:
	Menu(void);
	~Menu(void);

	std::vector<MenuOption*> lMenuOption; 

	int activeMenuOption;
	int numOfMenuOptions;

	virtual void Update();
	virtual void Draw(SDL_Renderer* renderer);

	virtual void updateActiveButton(int iDir);
};

#endif