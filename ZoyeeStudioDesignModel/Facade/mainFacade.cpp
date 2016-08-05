#include "Facade.hpp"

int main()
{
	NormalCar* car  = new NormalCar;
	car->GetAirCondition()->Show();
	car->GetCarBase()->Show();

	VIPCar* vipCar = new VIPCar;
	vipCar->GetAirCondition()->Show();
	vipCar->GetCarBase()->Show();
	vipCar->GetIceBox()->Show();
	vipCar->GetMusicBox()->Show();
}