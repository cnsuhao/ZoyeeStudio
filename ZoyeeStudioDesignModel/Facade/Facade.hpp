#ifndef __FACADE_H__
#define __FACADE_H__

#include <iostream>
#include "carComponent.hpp"


class NormalCar
{
public:
	NormalCar(){
		pCarBase = new BaseCar;
		pAirCondition = new AirCondition;
	};

	IBase* GetCarBase(){
		return pCarBase;
	};

	IBase* GetAirCondition(){
		return pAirCondition;
	}
protected:
	IBase* pCarBase;
	IBase* pAirCondition;
};

class VIPCar
{
public:
	VIPCar(){
		pCarBase = new BaseCar;
		pAirCondition = new AirCondition;
		pMusicBox = new MusicBox;
		pIceBox = new IceBox;
	}

	IBase* GetCarBase(){
		return pCarBase;
	};

	IBase* GetAirCondition(){
		return pAirCondition;
	}

	IBase* GetMusicBox(){
		return pMusicBox;
	}

	IBase* GetIceBox(){
		return pIceBox;
	}

private:
	IBase* pCarBase;
	IBase* pAirCondition;
	IBase* pMusicBox;
	IBase* pIceBox;
};

#endif