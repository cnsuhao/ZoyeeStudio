#include "LihuaCat.hpp"
#include "PussyCat.hpp"

int main()
{
	ICatHouse* pLihuaHouse = new LihuaCatHouse;
	IProduct* pLihuaCat = pLihuaHouse->SellCat();
	IProduct* pLihuaFood = pLihuaHouse->SellFood();
	pLihuaCat->Show();
	pLihuaFood->Show();

	ICatHouse* pPussyHouse = new PussyCatHouse;
	IProduct* pPussyCat = pPussyHouse->SellCat();
	IProduct* pPussyFood = pPussyHouse->SellFood();
	pPussyCat->Show();
	pPussyFood->Show();
}