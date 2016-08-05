#include "interpreter.hpp"

int main()
{
	Context context("Hello world");
	iInterpreter* pPc = new PCInterpreter(&context);
	iInterpreter* pMobile = new MobileInterpreter(&context);

	pPc->ShowText();
	pMobile->ShowText();
}

