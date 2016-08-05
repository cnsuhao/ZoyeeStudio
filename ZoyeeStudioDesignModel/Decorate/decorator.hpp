#ifndef __DECORATOR_H__
#define __DECORATOR_H__

#include "player.hpp"

class IDecorator
{
public:
	IDecorator(MediaPlayer* pMediaPlayer){
		this->pMediaPlayer = pMediaPlayer;
	};
	virtual void BeforePlay(){};
protected:
	MediaPlayer* pMediaPlayer;
};

class PlayAdv : public IDecorator
{
public:
	PlayAdv(MediaPlayer* pPlayer) : IDecorator(pPlayer){	};
	void BeforePlay(){
		printf("Play a advertisement\n");
		pMediaPlayer->Play();
	};
};

class PlayCopyright : public IDecorator
{
public:
	PlayCopyright(MediaPlayer* pPlayer) : IDecorator(pPlayer){ };
	void BeforePlay(){
		printf("Play a copyright\n");
		pMediaPlayer->Play();
	}
};
#endif