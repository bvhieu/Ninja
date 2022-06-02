#include "MapLevel.h"

MapLevel::MapLevel(void) { }

MapLevel::MapLevel(int iBlockID) {
	this->iBlockID = iBlockID;
	this->iYPos = 0;
}

MapLevel::~MapLevel(void) { }

int MapLevel::updateYPos() {
	return iYPos;
}

int MapLevel::getBlockID() {
	return iBlockID;
}

void MapLevel::setBlockID(int iBlockID) {
	this->iBlockID = iBlockID;
}

