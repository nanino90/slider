#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <iostream>
#include <string>
#include <chrono>
#include <thread>
#include "parse.h"
#include "context.h"
#include "status.h"


bool parse(context* cont, int argc, char** argv)
{
	if( argc > 1)
	{
		std::string command(argv[1]);	
		if(command == "move")
		{
			if(argc==4)
			{
			cont->speed=std::stoi(argv[3]);	
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
				cont->status = STATUS_FINISHED;
				return 1;
			}
		}
		if(command == "manual")
		{
			if(argc==6)
			{

				std::string data(argv[2]);	
				cont->mode = MODE_PROGRAM;
				cont->status = STATUS_IDLE;
			}
			else
			{
				cont->status = STATUS_FINISHED;
				return 1;
			}
		}

	}
	else
	{

	}



	return true;
}
