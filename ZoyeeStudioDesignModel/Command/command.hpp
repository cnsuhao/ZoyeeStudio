#ifndef __COMMAND_H__
#define __COMMAND_H__

#include <iostream>

class Cook
{
public:
	void MakeMeet(){
		printf("Makemeet\n");
	}

	void MakeRice(){
		printf("Makerice\n");
	}
};

class ICommand
{
public:
	ICommand(Cook* pCook){
		this->pCook = pCook;
	};
	virtual void Excute() = 0;
protected:
	Cook* pCook;
};

class MeetCommand : public ICommand
{
public:
	MeetCommand(Cook* pCook) : ICommand(pCook){};
	void Excute(){
		pCook->MakeMeet();
	}
};

class RiceCommand : public ICommand
{
public:
	RiceCommand(Cook* pCook) : ICommand(pCook){};
	void Excute(){
		pCook->MakeRice();
	}
};

class Mother
{
public:
	static Mother* CallMother(){
		static Mother mother;
		return &mother;
	}

	void CallCook(ICommand* pCookCommand){
		pCookCommand->Excute();
	};
};

#endif