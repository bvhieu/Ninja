#pragma once

#ifndef MAINMENU_H
#define MAINMENU_H

#include "Menu.h"

class MainMenu : public Menu
{
public:
	MainMenu(void);
	~MainMenu(void);

	void Update();
	void Draw(SDL_Renderer *renderer);
	void enter();
	void updateActiveButton(int iDir);
};

#endif