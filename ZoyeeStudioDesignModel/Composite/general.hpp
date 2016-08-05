#ifndef __GENERAL_H__
#define __GENERAL_H__

#include "person.hpp"

class General : public IPerson
{
public:
	General(std::string strName) : IPerson(strName){

	}
	void Action(){
		printf("I am a general, \nI have some soldiers: ");
		for (int i = 0; i < pGroup.size(); i++)
		{
			printf("[ %s ]", pGroup[i]->GetName().c_str());
		}
		printf("\n");
	}
};

#endif