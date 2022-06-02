#pragma once

#ifndef MENUGUIDE_H
#define MENUGUIDE_H

#include "Menu.h"

class MenuGuide : public Menu
{
public:
	MenuGuide(void);
	~MenuGuide(void);

	void Draw(SDL_Renderer* renderer);

	void escape();
};

#endif