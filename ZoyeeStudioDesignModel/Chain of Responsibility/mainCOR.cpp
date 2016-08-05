#include "COR.hpp"

int main()
{
	Person* father = new Father("father", nullptr);
	Person* mother = new Mother("mother", father);
	Person* son = new Son("son", mother);

	Goods Rice(5);
	son->CarryGoods(&Rice);

	Goods ToolsBox(35);
	son->CarryGoods(&ToolsBox);

	Goods Car(2000);
	son->CarryGoods(&Car);
}