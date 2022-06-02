#include "header.h"
#include "Core.h"
#include "Image.h"
#include "CommonFunctions.h"
#include "Text.h"
#include "SDL_mixer.h"

Map* Core::oMap = new Map();
bool Core::quitGame = false;

bool Core::movePressed = false;
bool Core::keyMenuPressed = false;
bool Core::keyS = false;
bool Core::keyW = false;
bool Core::keyA = false;
bool Core::keyD = false;
bool Core::keyShift = false;
bool Core::keyAPressed = false;
bool Core::keyDPressed = false;

Core::Core(void) {
	this->quitGame = false;
	this->iFPS = 0;
	this->iNumOfFPS = 0;
	this->lFPSTime = 0;

	SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_AUDIO);
	window = SDL_CreateWindow("Ninja", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, CommonFunctions::GAME_WIDTH, CommonFunctions::GAME_HEIGHT, SDL_WINDOW_SHOWN);
	if(window == NULL) {
		quitGame = true;
	}
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

	// Icon
	std::string fileName = "assets/images/icon.bmp";
	SDL_Surface* loadedSurface = SDL_LoadBMP(fileName.c_str());
	SDL_SetColorKey(loadedSurface, SDL_TRUE, SDL_MapRGB(loadedSurface->format, 255, 0, 255));
	SDL_SetWindowIcon(window, loadedSurface);
	SDL_FreeSurface(loadedSurface);
	mainEvent = new SDL_Event();
	
	Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);
	// Main Menu background
	oMap = new Map(renderer);
	CommonFunctions::getMenuManager()->setActiveOption(renderer);
	CommonFunctions::getMenuBackground()->setIMG("menuBackground", renderer);
	CommonFunctions::getMenuGuide()->setIMG("menuGuide", renderer);

	CommonFunctions::getMusic()->PlayMusic();

	this->keyMenuPressed = this->movePressed = this->keyS = this->keyW = this->keyA = this->keyD = this->keyShift = false;
	this->keyAPressed = this->keyDPressed = this->firstDir = false;

	CommonFunctions::keyIDA = SDLK_a;
	CommonFunctions::keyIDS = SDLK_s;
	CommonFunctions::keyIDD = SDLK_d;
	CommonFunctions::keyIDSpace = SDLK_SPACE;
	CommonFunctions::keyIDShift = SDLK_LSHIFT;
}

Core::~Core(void) {
	delete oMap;
	delete mainEvent;
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
}

void Core::mainLoop() {
	lFPSTime = SDL_GetTicks();

	while(!quitGame && mainEvent->type != SDL_QUIT) {
		frameTime = SDL_GetTicks();
		SDL_PollEvent(mainEvent);
		SDL_RenderClear(renderer);

		CommonFunctions::getMenuManager()->setBackgroundColor(renderer);
		
		Input();
		Update();
		Draw();
		SDL_RenderPresent(renderer);
		
		if(SDL_GetTicks() - frameTime < MIN_FRAME_TIME) {
			SDL_Delay(MIN_FRAME_TIME - (SDL_GetTicks () - frameTime));
		}
	}
}

void Core::Input() {
	switch(CommonFunctions::getMenuManager()->getViewID()) {
		case 2:
			if(!oMap->getInEvent()) {
				InputPlayer();
			} else {
				resetMove();
			}
			break;
		default:
			InputMenu();
			break;
	}
}

void Core::InputMenu() {
	if(mainEvent->type == SDL_KEYDOWN) {
		CommonFunctions::getMenuManager()->setKey(mainEvent->key.keysym.sym);

		switch(mainEvent->key.keysym.sym) {
			case SDLK_s: case SDLK_DOWN:
				if(!keyMenuPressed) {
					CommonFunctions::getMenuManager()->keyPressed(2);
					keyMenuPressed = true;
				}
				break;
			case SDLK_w: case SDLK_UP:
				if(!keyMenuPressed) {
					CommonFunctions::getMenuManager()->keyPressed(0);
					keyMenuPressed = true;
				}
				break;
			case SDLK_KP_ENTER: case SDLK_RETURN:
				if(!keyMenuPressed) {
					CommonFunctions::getMenuManager()->enter();
					keyMenuPressed = true;
				}
				break;
			case SDLK_ESCAPE:
				if(!keyMenuPressed) {
					CommonFunctions::getMenuManager()->escape();
					keyMenuPressed = true;
				}
				break;
			case SDLK_LEFT: case SDLK_d:
				if(!keyMenuPressed) {
					CommonFunctions::getMenuManager()->keyPressed(3);
					keyMenuPressed = true;
				}
				break;
			case SDLK_RIGHT: case SDLK_a:
				if(!keyMenuPressed) {
					CommonFunctions::getMenuManager()->keyPressed(1);
					keyMenuPressed = true;
				}
				break;
		}
	}

	if(mainEvent->type == SDL_KEYUP) {
		switch(mainEvent->key.keysym.sym) {
			case SDLK_s: case SDLK_DOWN: case SDLK_w: case SDLK_UP: case SDLK_KP_ENTER: case SDLK_RETURN: case SDLK_ESCAPE: case SDLK_a: case SDLK_RIGHT: case SDLK_LEFT: case SDLK_d:
				keyMenuPressed = false;
				break;
			default:
				break;
		}
	}
}

void Core::InputPlayer() {
	if(mainEvent->type == SDL_WINDOWEVENT) {
		switch(mainEvent->window.event) {
			case SDL_WINDOWEVENT_FOCUS_LOST:
				CommonFunctions::getMenuManager()->resetActiveOptionID(CommonFunctions::getMenuManager()->ePasue);
				CommonFunctions::getMenuManager()->setViewID(CommonFunctions::getMenuManager()->ePasue);
				CommonFunctions::getMusic()->PlayChunk(CommonFunctions::getMusic()->cPASUE);
				CommonFunctions::getMusic()->PauseMusic();
				break;
		}
	}

	if(mainEvent->type == SDL_KEYUP) {
		if(mainEvent->key.keysym.sym == CommonFunctions::keyIDD) {
				if(firstDir) {
					firstDir = false;
				}
				keyDPressed = false;
			}
		
			if(mainEvent->key.keysym.sym == CommonFunctions::keyIDA) {
				if(!firstDir) {
					firstDir = true;
				}
				keyAPressed = false;
			}
		
			if(mainEvent->key.keysym.sym == CommonFunctions::keyIDSpace) {
				CommonFunctions::keySpace = false;
			}

			if(mainEvent->key.keysym.sym == CommonFunctions::keyIDShift) {
				if(keyShift) {
					oMap->getPlayer()->resetDart();
					keyShift = false;
				}
			}
		
		switch(mainEvent->key.keysym.sym) {
			case SDLK_KP_ENTER: case SDLK_RETURN: case SDLK_ESCAPE:
				keyMenuPressed = false;
				break;
		}
	}

	if(mainEvent->type == SDL_KEYDOWN) {
		if(mainEvent->key.keysym.sym == CommonFunctions::keyIDD) {
			keyDPressed = true;
			if(!keyAPressed) {
				firstDir = true;
			}
		}
		
		if(mainEvent->key.keysym.sym == CommonFunctions::keyIDA) {
			keyAPressed = true;
			if(!keyDPressed) {
				firstDir = false;
			}
		}
		
		if(mainEvent->key.keysym.sym == CommonFunctions::keyIDSpace) {
			if(!CommonFunctions::keySpace) {
				oMap->getPlayer()->jump();
				CommonFunctions::keySpace = true;
			}
		}

		if(mainEvent->key.keysym.sym == CommonFunctions::keyIDShift) {
			if(!keyShift) {
				oMap->getPlayer()->fightDart();
				keyShift = true;
			}
		}

		switch(mainEvent->key.keysym.sym) {
			case SDLK_KP_ENTER: case SDLK_RETURN:
				if(!keyMenuPressed) {
					CommonFunctions::getMenuManager()->enter();
					keyMenuPressed = true;
				}
			case SDLK_ESCAPE:
				if(!keyMenuPressed && CommonFunctions::getMenuManager()->getViewID() == CommonFunctions::getMenuManager()->eGame) {
					CommonFunctions::getMenuManager()->resetActiveOptionID(CommonFunctions::getMenuManager()->ePasue);
					CommonFunctions::getMenuManager()->setViewID(CommonFunctions::getMenuManager()->ePasue);
					CommonFunctions::getMusic()->PlayChunk(CommonFunctions::getMusic()->cPASUE);
					CommonFunctions::getMusic()->PauseMusic();
					keyMenuPressed = true;
				}
				break;
		}
	}

	if(keyAPressed) {
		if(!oMap->getPlayer()->getMove() && firstDir == false && !oMap->getPlayer()->getChangeMoveDirection() ) {
			oMap->getPlayer()->startMove();
			oMap->getPlayer()->setMoveDirection(false);
		} else if(!keyDPressed && oMap->getPlayer()->getMoveSpeed() > 0 && firstDir != oMap->getPlayer()->getMoveDirection()) {
			oMap->getPlayer()->setChangeMoveDirection();
		}
	}

	if(keyDPressed) {
		if(!oMap->getPlayer()->getMove() && firstDir == true && !oMap->getPlayer()->getChangeMoveDirection() ) {
			oMap->getPlayer()->startMove();
			oMap->getPlayer()->setMoveDirection(true);
		} else if(!keyAPressed && oMap->getPlayer()->getMoveSpeed() > 0 && firstDir != oMap->getPlayer()->getMoveDirection()) {
			oMap->getPlayer()->setChangeMoveDirection();
		}
	}

	if(oMap->getPlayer()->getMove() && !keyAPressed && !keyDPressed) {
		oMap->getPlayer()->resetMove();
	}
}

void Core::resetKeys() {
	movePressed = keyMenuPressed = keyS = keyW = keyA = keyD = CommonFunctions::keySpace = keyShift = keyAPressed = keyDPressed = false;
}

void Core::Update() {
	CommonFunctions::getMenuManager()->Update();
}


void Core::Draw() {
	CommonFunctions::getMenuManager()->Draw(renderer);
}

void Core::resetMove() {
	this->keyAPressed = this->keyDPressed = false;
}

Map* Core::getMap() {
	return oMap;
}