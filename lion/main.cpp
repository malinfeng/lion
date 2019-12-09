#include <cstdio>
#include "Server.h"
#include "TestEventFactory.h"

using namespace lion;

int main()
{
	EventFactory *factory = new TestEventFactory();
	Server *myserver = new Server(8000, 4, factory);
	myserver->start();
    return 0;
}