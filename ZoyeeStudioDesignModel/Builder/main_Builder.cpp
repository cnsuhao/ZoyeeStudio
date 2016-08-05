#include "Cat.h"

int  main()
{
	IBuilder* pCatMother = new CatMother;
	pCatMother->Create();
	CatBaby* pBaby = pCatMother->GetBaby();
	pBaby->Show();
}