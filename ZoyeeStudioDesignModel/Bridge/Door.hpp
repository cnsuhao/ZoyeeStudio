#ifndef __DOOR_H__
#define __DOOR_H__

#include "Model.hpp"

class MetalDoor : public IDoor
{
public:
	void Show(){
		printf("this is a metal door\n");
	}
};

class WoodDoor : public IDoor
{
public:
	void Show(){
		printf("this is a wood door\n");
	}
};

#endif