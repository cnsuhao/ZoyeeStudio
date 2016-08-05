#include "Cat.hpp"

int main()
{
	IPrototype* pCat1 = new Cat();
	pCat1->Show();
	IPrototype* pCat2 = pCat1->Clone();
	pCat2->Show();
}