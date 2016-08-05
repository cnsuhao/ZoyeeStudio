#ifndef __SOLDIDER_H__
#define __SOLDIDER_H__

#include "person.hpp"

class Soldier : public IPerson
{
public:
	Soldier(std::string strName) : IPerson(strName){

	}
	void Action(){
		printf("I am a soldier named %s\n", strName.c_str());
	}
};

#endif