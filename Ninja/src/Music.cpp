#include "Music.h"
#include "Core.h"
#include "SDL_mixer.h"

Music::Music(void){
	Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);

	vMusic.push_back(loadMusic("musicBackg"));

	vChunk.push_back(loadChunk("death"));
	vChunk.push_back(loadChunk("gameover"));
	vChunk.push_back(loadChunk("jump"));
	vChunk.push_back(loadChunk("pause"));
	vChunk.push_back(loadChunk("fire"));
	vChunk.push_back(loadChunk("stomp"));
	setVolume(100);
}

Music::~Music(void){
	Mix_FreeMusic(vMusic[0]);
	vMusic.clear();

	for (unsigned int i = 0; i < vChunk.size(); i++){
		Mix_FreeChunk(vChunk[i]);
	}
	vChunk.clear();
}

void Music::PlayMusic(){
		Mix_PlayMusic(vMusic[0], -1);
		musicStopped = false;
}

void Music::StopMusic(){
	if (!musicStopped){
		Mix_HaltMusic();
		musicStopped = true;
	}
}

void Music::PauseMusic(){
	if (Mix_PausedMusic() == 1)
	{
		Mix_ResumeMusic();
		musicStopped = false;
	}
	else
	{
		Mix_PauseMusic();
		musicStopped = true;
	}
}

void Music::PlayChunk(eChunk chunkID){
	Mix_VolumeChunk(vChunk[chunkID], iVolume);
	Mix_PlayChannel(-1, vChunk[chunkID], 0);
}

Mix_Music *Music::loadMusic(std::string fileName){
	fileName = "assets/sounds/" + fileName + ".wav";
	return Mix_LoadMUS(fileName.c_str());
}

Mix_Chunk *Music::loadChunk(std::string fileName){
	fileName = "assets/sounds/" + fileName + ".wav";
	return Mix_LoadWAV(fileName.c_str());
}

int Music::getVolume(){
	return iVolume;
}

void Music::setVolume(int iVolume){
	this->iVolume = iVolume;
	Mix_VolumeMusic(iVolume);
}
