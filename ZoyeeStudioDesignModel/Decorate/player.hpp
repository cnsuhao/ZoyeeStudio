#ifndef __PLAYER_H__
#define __PLAYER_H__

#include <iostream>

class MediaPlayer
{
public:
	virtual void Play(){};
};

class RMPlayer : public MediaPlayer
{
public:
	void Play(){
		printf("%s\n", __FUNCTION__);
	}
};

class MP4Player : public MediaPlayer
{
public:	 
	void Play(){
		printf("%s\n", __FUNCTION__);
	}
};

#endif