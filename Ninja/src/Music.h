#pragma once

#ifndef MUSIC_H
#define MUSIC_H

#include "SDL_mixer.h"
#include <string>
#include <vector>

class Music
{
private:
	std::vector<Mix_Music *> vMusic;
	std::vector<Mix_Chunk *> vChunk;

	int iVolume;

public:
	Music(void);
	~Music(void);

	bool musicStopped;
	enum eMusic{
		mMUSICBACKG,
	};

	enum eChunk{
		cDEATH,
		cGAMEOVER,
		cJUMP,
		cPASUE,
		cFIRE,
		cSTOMP,
	};

	void PlayMusic();
	void StopMusic();
	void PauseMusic();

	void PlayChunk(eChunk chunkID);

	Mix_Music *loadMusic(std::string fileName);
	Mix_Chunk *loadChunk(std::string fileName);

	int getVolume();
	void setVolume(int iVolume);
};

#endif