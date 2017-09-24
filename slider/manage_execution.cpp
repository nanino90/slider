#include "manage_execution.h"
#include "status.h"
/*
void manage_execution(context* cont)
{

		switch(cont->status)
		{
			case STATUS_INIT:
				std::cout<<"Init"<<std::endl;
				cont->car.set_run1(1,1000,2,1000);
				cont->car.print_config();
				if(cont->car.validate_config())
				{
					std::cout<<"Imposible parameters!"<<std::endl;
					cont->status = STATUS_INIT;
				}
				cont->status = STATUS_CONFIGURED;
				break;
			case STATUS_CONFIGURED:
				cont->status = STATUS_TAKING;
				break;
			case STATUS_MOVING:
				std::cout<<"Moving"<<std::endl;
				if( cont->car.move() )
					cont->status = STATUS_WAITING;
				break;
			case STATUS_WAITING:
				std::cout<<"Waiting"<<std::endl;
				if( cont->car.wait() )
					cont->status = STATUS_TAKING;
			break;
			case STATUS_TAKING:
				std::cout<<"Taking"<<std::endl;
				if( cont->car.take() )
					cont->status = STATUS_MOVING;
				break;
			case STATUS_FINISH:
				std::cout<<"Finish"<<std::endl;
				break;
			default:
				std::cout<<"Default"<<std::endl;
				break;

		}
		if(cont->car.time_step())
			cont->status=STATUS_FINISH;	
}*/
