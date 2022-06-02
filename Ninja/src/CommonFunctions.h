#pragma once

#ifndef COMMONFUNCTIONS_H
#define COMMONFUNCTIONS_H

#include "Text.h"
#include "MenuManager.h"
#include "Music.h"

class CommonFunctions
{
private:
	static Text* oText;
	static MenuManager* oMenuManager;
	static Image* tMenuBackground;
	static Image* tMenuGuide;
	static Music* oMusic;
public:
	CommonFunctions(void);
	~CommonFunctions(void);

	static int GAME_WIDTH, GAME_HEIGHT;

	static bool keySpace;
	static int keyIDA, keyIDS, keyIDD, keyIDShift, keyIDSpace;

	static Text* getText();
	static MenuManager* getMenuManager();
	static Image* getMenuBackground();
	static Image* getMenuGuide();
	static Music* getMusic();

	static std::string getKeyString(int keyID);
};

#endif
