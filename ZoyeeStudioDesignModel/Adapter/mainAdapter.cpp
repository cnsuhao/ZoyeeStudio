#include "MouseTools.hpp"

int main()
{
	MouseTool* pTool = new MouseTool;	
	MouseTool* pCatTool = new Tool(new Cat);
	pTool->CatchMouse();
	pCatTool->CatchMouse();
}