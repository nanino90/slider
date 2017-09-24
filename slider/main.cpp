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
#include "parse.h"

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
				parse(cont, argc, argv);
				if(cont->mode==MODE_PROGRAM)
				{
					if( !car.set_program(
								std::stoi(std::string(argv[2])),
								std::stoi(std::string(argv[3])),
								std::stoi(std::string(argv[4])),
								std::stoi(std::string(argv[5])) ) )
					{
						std::cout<<"Impossible values"<<std::endl;
						cont->status = STATUS_FINISHED;
					}
				}
				break;
			case STATUS_IDLE:
				cont->status = STATUS_RUNNING;
				car.print_config();
				//wait for init signal

				break;
				//running mode
			case STATUS_RUNNING:
				switch(cont->mode)
				{
					case MODE_PROGRAM:
						car.program();
						break;
					case	MODE_TO_END:
						car.move(DIR::END, cont->speed);
						break;
					case	MODE_TO_START:
						car.move(DIR::START, cont->speed);
						break;
					case	MODE_MANUAL:
						break;
					default:
						break;
				}	
				break;
				//program finished
			case STATUS_FINISHED:

				return 0;
				break;

		}
		if(car.m_prog == PROG_FINISH)
		{
			return 0;

		}
		std::this_thread::sleep_for(std::chrono::milliseconds(1));
		//car.print_status();
	}
	return 0;

}


