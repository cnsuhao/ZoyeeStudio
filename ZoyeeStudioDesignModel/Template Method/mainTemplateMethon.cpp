#include "TemplateMethod.hpp"

int main()
{
	ICar* pCar = new Benz;
	pCar->RunTemplateMethod();

	ICar* pCar2 = new Bmw;
	pCar2->RunTemplateMethod();
}