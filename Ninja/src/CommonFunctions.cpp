#include "CommonFunctions.h"

CommonFunctions::CommonFunctions(void) { }

CommonFunctions::~CommonFunctions(void) {
	delete oText;
	delete oMenuManager;
	delete tMenuBackground;
	delete tMenuGuide;
	delete oMusic;
}

int CommonFunctions::GAME_WIDTH  = 800;
int CommonFunctions::GAME_HEIGHT = 448;

Text* CommonFunctions::oText = new Text();
MenuManager* CommonFunctions::oMenuManager = new MenuManager();
Image* CommonFunctions::tMenuBackground = new Image();
Image* CommonFunctions::tMenuGuide = new Image();
Music* CommonFunctions::oMusic = new Music();

bool CommonFunctions::keySpace = false;

int CommonFunctions::keyIDA = 0;
int CommonFunctions::keyIDD = 0;
int CommonFunctions::keyIDS = 0;
int CommonFunctions::keyIDShift = 0;
int CommonFunctions::keyIDSpace = 0;

Text* CommonFunctions::getText() {
	return oText;
}

MenuManager* CommonFunctions::getMenuManager() {
	return oMenuManager;
}

Image* CommonFunctions::getMenuBackground() {
	return tMenuBackground;
}

Image* CommonFunctions::getMenuGuide() {
	return tMenuGuide;
}

Music* CommonFunctions::getMusic() {
	return oMusic;
}

std::string CommonFunctions::getKeyString(int keyID) {
	if(keyID >= 97 && keyID <= 122) {
		return std::string(1, '0' + (keyID - 32) - 48);
	}

	if(keyID >= 48 && keyID <= 57) {
		return std::string(1, '0' + (keyID - 32) - 48);
	}

	switch(keyID) {
		case SDLK_ESCAPE:
			return "ESCAPE";
		case SDLK_SPACE:
			return "SPACE";
		case SDLK_UP:
			return "UP";
		case SDLK_DOWN:
			return "DOWN";
		case SDLK_RIGHT:
			return "RIGHT";
		case SDLK_LEFT:
			return "LEFT";
		case SDLK_LSHIFT:
			return "SHIFT";
		case SDLK_RSHIFT:
			return "SHIFT";
	}
	return "NONE";
}