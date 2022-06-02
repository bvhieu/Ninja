// #include "Points.h"
// #include "CommonFunctions.h"
// #include "Core.h"

// Points::Points(int iXPos, int iYPos, std::string sText){
// 	this->iXPos = iXPos;
// 	this->iYPos = iYPos;
// 	this->sText = sText;
// 	this->iYLEFT = 96;

// 	this->iW = 8;
// 	this->iH = 16;

// 	this->bDelete = false;
// }

// Points::Points(int iXPos, int iYPos, std::string sText, int iW, int iH){
// 	this->iXPos = iXPos;
// 	this->iYPos = iYPos;
// 	this->sText = sText;

// 	this->iW = iW;
// 	this->iH = iH;

// 	this->bDelete = false;
// }

// Points::~Points(void){ }

// void Points::Update(){

// }

// void Points::Draw(SDL_Renderer *renderer){
// 	CommonFunctions::getText()->Draw(renderer, sText, iXPos + (int)Core::getMap()->getXPos(), iYPos, iW, iH);
// }

// bool Points::getDelete(){
// 	return bDelete;
// }