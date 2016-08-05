#include "flyweight.hpp"

int main()
{
	FlyweightFactory factory;
	IFlyweight* ptrHello = factory.GetQuickData("Hello");	
	IFlyweight* ptrWorld = factory.GetQuickData("World");
	IFlyweight* ptrHello2 = factory.GetQuickData("Hello");

	ptrHello->Show();
	ptrWorld->Show();
	ptrHello2->Show();
}