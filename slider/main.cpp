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

#define DEBUG

int main (int argc, char *argv[])
{

	std::cout<<"Init program"<<std::endl;  	

	context* cont = get_context();

	context_construct(cont);

	cart car;	


	for(;;)
	{
		//Input buttons
		if(1)				
		{

		}

		//Manage display

		//Execution
		switch(cont->status)
		{
			case STATUS_INIT:
#ifdef DEBUG
				std::cout<<"Init"<<std::endl;
				car.set_run1(10,1000,5,100);
				car.print_config();
				if(car.validate_config())
				{
					std::cout<<"Imposible parameters!"<<std::endl;
					cont->status = STATUS_INIT;
				}
				cont->status = STATUS_CONFIGURED;
#endif
				break;
			case STATUS_CONFIGURED:
				cont->status = STATUS_TAKING;
				break;
			case STATUS_MOVING:
				std::cout<<"Moving"<<std::endl;
				if( car.move() )
					cont->status = STATUS_WAITING;
				break;
			case STATUS_WAITING:
				std::cout<<"Waiting"<<std::endl;
				if( car.wait() )
					cont->status = STATUS_TAKING;
			break;
			case STATUS_TAKING:
				std::cout<<"Taking"<<std::endl;
				if( car.take() )
					cont->status = STATUS_MOVING;
				break;
			case STATUS_FINISH:
				std::cout<<"Finish"<<std::endl;
				break;
			default:
				std::cout<<"Default"<<std::endl;
				break;

		}

		std::this_thread::sleep_for(std::chrono::milliseconds(1));

#ifdef DEBUG	
		std::this_thread::sleep_for(std::chrono::milliseconds(10));
#endif
		car.time_step();	
}




	return 0;
}

