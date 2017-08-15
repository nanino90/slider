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

int main (int argc, char *argv[])
{

	uint32_t time=0;
	std::cout<<"Init program"<<std::endl;  	

	context* cont = get_context();

	context_construct(cont);

	cart car;	


	for(;;)
	{

		switch(cont->status)
		{
			case STATUS_INIT:
				std::cout<<"Init"<<std::endl;

				car.set_run1(10,1000,5,100);
				car.print_config();

				cont->status = STATUS_CONFIGURED;
				break;
			case STATUS_CONFIGURED:
//				std::cout<<"Configured"<<std::endl;
//				car.print_status();

				if( !(time%car.m_interval) )
				{
					cont->status = STATUS_TAKING;

				}	
				else if( !(time%car.m_velocidad) )
				{
					cont->status = STATUS_MOVING;

				}				
				 if(car.m_pos>=car.m_distancia)
					cont->status = STATUS_FINISH;
				break;
			case STATUS_MOVING:
				std::cout<<"Moving"<<std::endl;
				++car.m_pos;
				cont->status = STATUS_CONFIGURED;
				break;
			case STATUS_TAKING:
				std::cout<<"\tTaking"<<std::endl;
					cont->status = STATUS_CONFIGURED;
				break;
			case STATUS_FINISH:
				std::cout<<"Finish"<<std::endl;
				break;
			default:
				std::cout<<"Default"<<std::endl;
				break;

		}

		std::this_thread::sleep_for(std::chrono::milliseconds(10));
		++time;
	}




	return 0;
}

