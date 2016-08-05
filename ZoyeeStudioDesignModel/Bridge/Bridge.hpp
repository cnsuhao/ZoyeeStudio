#ifndef __BRIDGE_H__
#define __BRIDGE_H__

#include "Model.hpp"
#include "Window.hpp"
#include "Door.hpp"

class Bridge
{
public:
	void SetWindow(IWindow* pWindow){
		this->pWindow = pWindow;
	};
	void SetDoor(IDoor* pDoor){
		this->pDoor = pDoor;
	};
	void ShowWindow(){
		this->pWindow->Show();
	};
	void ShowDoor(){
		this->pDoor->Show();
	};
protected:
	IWindow* pWindow;
	IDoor* pDoor;
};

class WoodHouse : public Bridge
{
public:
	WoodHouse(){
		SetWindow(new WoodWindow);
		SetDoor(new WoodDoor);
	};
	void Show(){
		printf("this is a wood house\n");
		ShowDoor();
		ShowWindow();
	};
};

class MetalHouse : public Bridge
{
public:
	MetalHouse(){
		SetWindow(new MetalWindow);
		SetDoor(new MetalDoor);
	};
	void Show(){
		printf("this is a metal house\n");
		ShowDoor();
		ShowWindow();
	};
};

#endif
