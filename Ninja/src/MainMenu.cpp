#include "MainMenu.h"
#include "CommonFunctions.h"
#include "Core.h"

MainMenu::MainMenu(void){
	this->lMenuOption.push_back(new MenuOption("PLAY GAME", 315, 240));
	this->lMenuOption.push_back(new MenuOption("OPTIONS", 330, 272));
	this->lMenuOption.push_back(new MenuOption("GUIDE", 345, 304));

	this->numOfMenuOptions = lMenuOption.size();
}

MainMenu::~MainMenu(void){ }

void MainMenu::Update(){
	CommonFunctions::getMusic()->StopMusic();
	Menu::Update();
}

void MainMenu::Draw(SDL_Renderer *renderer){
	CommonFunctions::getMenuBackground()->Draw(renderer, 0, 0);
	Menu::Draw(renderer);
}

void MainMenu::enter(){
	switch (activeMenuOption)
	{
	case 0:
		Core::getMap()->resetGameData();
		CommonFunctions::getMenuManager()->setViewID(CommonFunctions::getMenuManager()->eGameLoading);
		CommonFunctions::getMenuManager()->getLoadingMenu()->loadingType = true;
		break;
	case 1:
		CommonFunctions::getMenuManager()->getOptions()->updateVolumeRect();
		CommonFunctions::getMenuManager()->setViewID(CommonFunctions::getMenuManager()->eOptions);
		break;
	case 2:
		CommonFunctions::getMenuManager()->setViewID(CommonFunctions::getMenuManager()->eGuide);
		break;
	}
}

void MainMenu::updateActiveButton(int iDir){
	Menu::updateActiveButton(iDir);
}