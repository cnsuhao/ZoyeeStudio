#ifndef __PROXY_H__
#define __PROXY_H__

#include <iostream>

class Image
{
public:
	virtual void Show(){};
};

class BigImage : public Image
{
public:
	void Show(){
		printf("%s\n", __FUNCTION__);
	}
};

class BigImageProxy : public Image
{
public:
	BigImageProxy() : pImage(0){
		
	}

	void Show(){
		if ( ! pImage)
		{
			pImage = new BigImage;
		}
		pImage->Show();
	}
protected:
	Image* pImage;
};

#endif