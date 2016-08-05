#ifndef __WINDOW_H__
#define __WINDOW_H__

#include "Model.hpp"

class MetalWindow : public IWindow
{
public:
	void Show(){
		printf("this is a metal window\n");
	}
};

class WoodWindow : public IWindow
{
public:
	void Show(){
		printf("this is a wood window\n");
	}
};

#endif