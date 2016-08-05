#include "command.hpp"

int main()
{
	Cook cook;
	ICommand* pRiceCmd = new RiceCommand(&cook);
	ICommand* pMeetCmd = new MeetCommand(&cook);

	Mother* mother = Mother::CallMother();
	mother->CallCook(pMeetCmd);
	mother->CallCook(pRiceCmd);
}