#ifndef __COR_H__
#define __COR_H__

#include <iostream>

class Goods
{
public:
	Goods(int nKg){
		m_nWeight = nKg;
	};
	int GetWeight(){
		printf("Goods is [%d] kg\n", m_nWeight);
		return m_nWeight;
	};
private:
	int m_nWeight;
};

class Person
{
public:
	Person(std::string strName, Person* pHelper = nullptr){
		m_pHelper = pHelper;
		this->strName = strName;
	};

	virtual void CarryGoods(Goods* pGoods) = 0;

	std::string GetName(){
		return strName;
	}
protected:
	Person* m_pHelper;
	std::string strName;
};

class Son : public Person
{
public:
	Son(std::string strName, Person* pHelper) : Person(strName, pHelper){};
	void CarryGoods(Goods* pGoods){
		if (pGoods->GetWeight() > 10)
		{
			printf("[%s] can't carry it, pass to [%s]\n", strName.c_str(), m_pHelper->GetName().c_str());
			m_pHelper->CarryGoods(pGoods);
		}
		else
		{
			printf("[%s] carry it!\n", strName.c_str());
		}
	}
};

class Mother : public Person
{
public:
	Mother(std::string strName, Person* pHelper) : Person(strName, pHelper){};
	void CarryGoods(Goods* pGoods){
		if (pGoods->GetWeight() > 30)
		{
			printf("[%s] can't carry it, pass to [%s]\n", strName.c_str(), m_pHelper->GetName().c_str());
			m_pHelper->CarryGoods(pGoods);
		}
		else
		{
			printf("[%s] carry it!\n", strName.c_str());
		}
	}
};

class Father : public Person
{
public:
	Father(std::string strName, Person* pHelper) : Person(strName, pHelper){};
	void CarryGoods(Goods* pGoods){
		if (pGoods->GetWeight() > 50)
		{
			printf("[%s] can't carry it, no body helps\n", strName.c_str());
		}
		else
		{
			printf("[%s] carry it!\n", strName.c_str());
		}
	}
};


#endif