#ifndef __CAR_COMPONENT_H__
#define __CAR_COMPONENT_H__

class IBase
{
public:
	virtual void Show(){};
};

class MusicBox : public IBase
{
public:
	void Show(){
		printf("%s\n", __FUNCTION__);
	}
};

class AirCondition : public IBase
{
public:
	void Show(){
		printf("%s\n", __FUNCTION__);
	}
};

class IceBox : public IBase
{
public:
	void Show(){
		printf("%s\n", __FUNCTION__);
	}
};

class BaseCar : public IBase
{
public:
	void Show(){
		printf("%s\n", __FUNCTION__);
	}
};

#endif