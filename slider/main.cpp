#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <iostream>
#include <string>
#include <chrono>
#include <thread>
#include "context.h"
#include "cart.h"

int main (int argc, char *argv[])
{
	std::cout<<"Init program"<<std::endl;  	

	context* cont;

	context_construct(cont);

	cart car;	


	for(;;)
	{
		std::cout<<"step"<<std::endl;

		switch(cont->status)
		{
			case 0:
				break;
			case 1:
				break;
			case 2:
				break;
			case 3:
				break;
			case 4:
				break;
			default:
				break;

		}

		std::this_thread::sleep_for(std::chrono::milliseconds(500));
	}




	return 0;
}

