#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <iostream>
#include <string>
#include <chrono>
#include <thread>
#include "context.h"
#include "cart.h"
#include "status.h"
#include "manage_execution.h"

#include <wiringPi.h>

#define DEBUG

int main (int argc, char *argv[])
{
	wiringPiSetup();

	std::cout<<"Init program"<<std::endl;  	

	context* cont = get_context();

	context_construct(cont);

	for(;;)
	{
		//Manage display
//		manage_execution(cont);

	if( cont->car.move_to_end(*argv[1]-'0')== false)
{
	return 0;
}	

		std::this_thread::sleep_for(std::chrono::milliseconds(1));

		if(cont->status == STATUS_FINISH)
			return 0;		
	}
	return 0;
}

