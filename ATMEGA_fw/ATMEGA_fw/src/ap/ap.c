#include "ap.h"

void apInit(void)
{
	sei();
}

void apMain(void)
{	
	while(true)
	{
		rosServerRun();
	}
}
