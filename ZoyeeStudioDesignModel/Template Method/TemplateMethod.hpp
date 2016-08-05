#ifndef __TEMPLATE_METHOD_H__
#define __TEMPLATE_METHOD_H__

#include <iostream>

class ICar
{
public:
	void RunTemplateMethod(){
		this->Start();
		this->Heat();
	};
	virtual void Start() = 0;
	virtual void Heat() = 0;
};

class Benz : public ICar
{
public:
	void Start(){
		printf("%s\n", __FUNCTION__);
	}

	void Heat(){
		printf("%s\n", __FUNCTION__);
	}
};

class Bmw : public ICar
{
public:
	void Start(){
		printf("%s\n", __FUNCTION__);
	}

	void Heat(){
		printf("%s\n", __FUNCTION__);
	}
};

#endif