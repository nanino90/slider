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
		//Input buttons
		if(1)				
		{

		}

		//Manage display
		manage_execution(cont);


		std::this_thread::sleep_for(std::chrono::milliseconds(1));
	//	std::this_thread::sleep_for(std::chrono::milliseconds(10));
	
	if(cont->status == STATUS_FINISH)
		return 0;		
	}




	return 0;
}

