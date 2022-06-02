#pragma once

#ifndef MAP_LEVEL_H
#define MAP_LEVEL_H

class MapLevel
{
private:
	int iBlockID;
	int iYPos;

public:
	MapLevel(void);
	MapLevel(int iBlockID);
	~MapLevel(void);

	int updateYPos();
	int getBlockID();
	void setBlockID(int iBlockID);
};

#endif