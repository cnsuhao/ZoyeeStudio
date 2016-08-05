#ifndef __FLYWEIGHT_H__
#define __FLYWEIGHT_H__

#include <iostream>
#include <string>
#include <map>

typedef std::string STATE;

class IFlyweight
{
public:
	STATE GetState(){
		return strInnerState;
	};

	void Show(){
		printf("[%p]State: %s", this, strInnerState.c_str());
	};
protected:
	IFlyweight(STATE strState) : strInnerState(strState){};
	STATE strInnerState;
};

class QuickData : public IFlyweight
{
public:
	QuickData(STATE strState) : IFlyweight(strState){};
};

class FlyweightFactory
{
public:
	IFlyweight* GetQuickData(STATE strState){
		std::map<STATE, IFlyweight*>::iterator iter = mapData.find(strState);
		if (iter != mapData.end())
		{
			return iter->second;
		}

		IFlyweight* pFlyweight = new QuickData(strState);
		mapData[strState] = pFlyweight;
		return pFlyweight;
	};

protected:
	std::map<STATE, IFlyweight*> mapData;
};

#endif