#include <iostream>
#include "LihuaCat.hpp"
#include "PussyCat.hpp"

int main()
{
	ICatHouse* lihuaCatHouse = new LihuaCatHouse;
	ICat* lihuaCat = lihuaCatHouse->Sell();
	lihuaCat->Action();

	ICatHouse* pussyCatHouse = new PussyCatHouse;
	ICat* pussyCat = pussyCatHouse->Sell();
	pussyCat->Action();
}