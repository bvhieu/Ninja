#pragma once

#ifndef CORE_H
#define CORE_H

#include "Map.h"

class Core
{
private:
	SDL_Window* window;
	SDL_Renderer* renderer;
	SDL_Event* mainEvent;

	// FPS 
	long frameTime;
	static const int MIN_FRAME_TIME = 16;
	unsigned long lFPSTime;
	int iNumOfFPS, iFPS;

	// INPUT
	static bool movePressed, keyMenuPressed, keyS, keyW, keyA, keyD, keyShift;
	static bool keyAPressed, keyDPressed;

	// true = RIGHT, false = LEFT
	bool firstDir;

	// INPUT
	static Map* oMap;

	// Methods
	void Input();
	void InputPlayer();
	void InputMenu();

public:
	Core(void);
	~Core(void);

	static bool quitGame;

	void mainLoop();

	void Update();
	void Draw();
	void resetMove();
	static void resetKeys();
	
	static Map* getMap();
};

#endif
