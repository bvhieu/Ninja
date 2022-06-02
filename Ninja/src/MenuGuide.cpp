#include "MenuGuide.h"
#include "CommonFunctions.h"
#include "Core.h"

MenuGuide::MenuGuide(void){ }

MenuGuide::~MenuGuide(void){ }

void MenuGuide::Draw(SDL_Renderer *renderer){
	CommonFunctions::getMenuGuide()->Draw(renderer, 0, 0);
}

void MenuGuide::escape(){
	CommonFunctions::getMenuManager()->setViewID(CommonFunctions::getMenuManager()->eMainMenu);
}
