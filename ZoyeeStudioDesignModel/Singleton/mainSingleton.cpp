#include "Cat.h"

int main()
{
	CatGod* pGod = CatGod::GetInstance();
	pGod->Show();
	CatGod* pGod2 = CatGod::GetInstance();
	pGod2->Show();
}