#include "general.hpp"
#include "soldier.hpp"
#include "King.hpp"

int main()
{
	IPerson* pKing = new King("King");
	IPerson* pGeneralA = new General("General-A");
	IPerson* pGeneralB = new General("General-B");
	pKing->Add(pGeneralA);
	pKing->Add(pGeneralB);
	pKing->Action();

	IPerson* pSoldierA1 = new Soldier("SoldierA-1");
	pSoldierA1->Action();
	IPerson* pSoldierA2 = new Soldier("SoldierA-2");	
	pSoldierA2->Action();
	pGeneralA->Add(pSoldierA1);
	pGeneralA->Add(pSoldierA2);
	pGeneralA->Action();

	IPerson* pSoldierB1 = new Soldier("SoldierB-1");
	pSoldierB1->Action();
	IPerson* pSoldierB2 = new Soldier("SoldierB-2");
	pSoldierB2->Action();
	pGeneralB->Add(pSoldierB1);
	pGeneralB->Add(pSoldierB2);
	pGeneralB->Action();
}