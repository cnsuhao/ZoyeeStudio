#ifndef __CAT_H__
#define __CAT_H__

#include "IBuilder.h"

class CatBaby
{
public:
	void SetHead(int type){
		Head = type;
	};
	void SetBody(int type){
		Body = type;
	};
	void SetLeg(int type){
		Leg = type;
	};
	void Show(){
		printf("Head: %d, Body: %d, Leg: %d\n", Head, Body, Leg);
	}
protected:
	int Head;
	int Body;
	int Leg;
};

class CatMother : public IBuilder
{
public:
	CatMother(){
		pBaby = new CatBaby;
	};
	virtual void Create(){
		BuildHead();
		BuildBody();
		BuildLeg();
	}
	
	CatBaby* GetBaby(){
		return pBaby;
	}

protected:
	virtual void BuildHead(){
		pBaby->SetHead(1);
	};
	virtual void BuildBody(){
		pBaby->SetBody(2);
	};
	virtual void BuildLeg(){
		pBaby->SetLeg(3);
	};
	CatBaby* pBaby;
};

#endif