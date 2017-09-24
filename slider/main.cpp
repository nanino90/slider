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

int main (int argc, char *argv[])
{
	wiringPiSetup();
	std::cout<<"Init program"<<std::endl;  	
	context* cont = get_context();
	context_construct(cont);
	cart car;

	for(;;)
	{
		switch(cont->status)
		{
			//read parameters and decide mode of operation
			case STATUS_INIT:
				if( argc > 1)
				{
					std::string command(argv[1]);	
					if(command == "manual")
					{
						if(argc>2)
						{

							std::string data(argv[2]);	
							if(data== "toend")
							{
								cont->mode=MODE_TO_END;
							}
							else if(data== "tostart")
							{
								cont->mode=MODE_TO_START;
							}
							cont->status = STATUS_IDLE;
						}
						else
						{
							std::cout<<"Missing command values"<<std::endl;
							return 1;
						}
					}
				}
				else
				{

				}

				break;
			case STATUS_IDLE:
				cont->status = STATUS_RUNNING;
				//wait for init signal

				break;
				//running mode
			case STATUS_RUNNING:
std::cout<<"sdf"<<std::endl;

				switch(cont->mode)
				{
					case MODE_PROGRAM:
						break;
					case	MODE_TO_END:
						car.move_to_limit(DIR::END);
						break;
					case	MODE_TO_START:
						car.move_to_limit(DIR::START);
						break;
					case	MODE_MANUAL:
						break;
					default:
						break;
				}	
				break;
				//program finished
			case STATUS_FINISHED:

				break;
		
}
if(car.m_prog == PROG_FINISH)
	{
	return 0;
	
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(1));
	}
	return 0;

}


