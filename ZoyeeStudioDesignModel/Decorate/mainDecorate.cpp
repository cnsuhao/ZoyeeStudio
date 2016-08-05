#include "decorator.hpp"

int main()
{
	MediaPlayer* pMp4 = new MP4Player;
	MediaPlayer* pRm = new RMPlayer;

	IDecorator* pAdvDecorator = new PlayAdv(pMp4);
	pAdvDecorator->BeforePlay();

	IDecorator* pCopyrightDecorator = new PlayCopyright(pRm);
	pCopyrightDecorator->BeforePlay();

}