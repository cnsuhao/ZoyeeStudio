#ifndef __PERSON_H__
#define __PERSON_H__

#include <vector>
#include <string>
#include <iostream>

class IPerson
{
public:
	IPerson(std::string strName){
		this->strName = strName;
	};

	std::string GetName(){
		return strName;
	};

	void Add(IPerson* person){
		pGroup.push_back(person);
	};

	void Remove(IPerson* person){
		std::vector<IPerson*>::iterator iter = pGroup.begin();
		for (; iter != pGroup.end(); iter++)
		{
			if (((IPerson*)(*iter))->GetName() == person->GetName())
			{
				pGroup.erase(iter);
				return;
			}
		}
	};

	IPerson* GetPerson(std::string strName){
		std::vector<IPerson*>::iterator iter = pGroup.begin();
		for (; iter != pGroup.end(); iter++)
		{
			if (((IPerson*)(*iter))->GetName() == strName)
			{
				return *iter;
			}
		}
		return nullptr;
	};

	virtual void Action(){};
protected:
	std::string strName;
	std::vector<IPerson*> pGroup;
};

#endif