#include "WakeupEvent.h"
#include "SocketOps.h"

using namespace lion;


void WakeupEvent::readEvent()
{
	uint64_t one = 1;
	ssize_t n = read(fd(), &one, sizeof one);
	if (n != sizeof one)
	{
		// log wakeup read n byte
	}
}

void WakeupEvent::writeEvent()
{
	//log wakeup write
	return;
}

void WakeupEvent::errorEvent()
{
	//log wakeup error
	return;
}

void WakeupEvent::closeEvent()
{
	//log wakeup close
	return;
}
