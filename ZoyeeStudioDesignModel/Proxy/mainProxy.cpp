#include "Proxy.hpp"

int main()
{
	Image* proxy = new BigImageProxy;
	proxy->Show();
}