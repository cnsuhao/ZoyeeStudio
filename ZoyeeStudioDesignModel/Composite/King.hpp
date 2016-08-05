#ifndef __KING_H__
#define __KING_H__

#include "person.hpp"

class King : public IPerson
{
public:
	King(std::string strName) : IPerson(strName){

	}
	void Action(){
		printf("I am KING!\nI have some generals:");
		for (int i = 0; i < pGroup.size(); i++)
		{
			printf("[ %s ]", pGroup[i]->GetName().c_str());
		}
		printf("\n");
	}
};

#endif