#include "Menu.h"
#include "CommonFunctions.h"

Menu::Menu(void){
	this->activeMenuOption = 0;
}

Menu::~Menu(void){ }

void Menu::Update(){ }

void Menu::Draw(SDL_Renderer *renderer){
	for (unsigned int i = 0; i < lMenuOption.size(); i++)
	{
		CommonFunctions::getText()->Draw(renderer, lMenuOption[i]->getText(), lMenuOption[i]->getXPos(), lMenuOption[i]->getYPos());
	}

	CommonFunctions::getMenuManager()->getActiveOption()->Draw(renderer, lMenuOption[activeMenuOption]->getXPos() - 32, lMenuOption[activeMenuOption]->getYPos());
}

void Menu::updateActiveButton(int iDir){
	switch (iDir)
	{
	case 0:
		if (activeMenuOption - 1 < 0)
		{
			activeMenuOption = numOfMenuOptions - 1;
		}
		else
		{
			--activeMenuOption;
		}
		break;
	case 2:
		if (activeMenuOption + 1 >= numOfMenuOptions)
		{
			activeMenuOption = 0;
		}
		else
		{
			++activeMenuOption;
		}
		break;
	default:
		break;
	}
}