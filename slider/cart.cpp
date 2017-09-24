#include "cart.h"
#include "status.h"
#include <wiringPi.h>

cart::cart(void):
	m_total_time(0),
	m_take_time(0),
	m_interval_time(0),
	m_sections(0),
	m_section_length(0),
	m_time(0),
	m_pos(0),
	m_count(0),
	m_stepper_pos(0),
	m_prog(PROG_INIT),
	m_action(ACTION_NONE)
{
	std::cout<<"Cart init"<<std::endl;
	pinMode((int)GPIO::OBT,OUTPUT);
	pinMode((int)GPIO::AA,OUTPUT);
	pinMode((int)GPIO::AB,OUTPUT);
	pinMode((int)GPIO::BA,OUTPUT);
	pinMode((int)GPIO::BB,OUTPUT);

}

cart::~cart(void)
{
	digitalWrite((int)GPIO::AA,0);
	digitalWrite((int)GPIO::AB,0);
	digitalWrite((int)GPIO::BA,0);
	digitalWrite((int)GPIO::BB,0);
	std::cout<<"Cart erased"<<std::endl;
}

uint16_t cart::get_pos(void)
{
	return m_pos;
}

bool cart::set_program(uint32_t obt,uint32_t duracion,uint16_t fotos, uint16_t distancia)
{
	m_total_time=duracion;
	m_take_time = obt;
	m_sections = fotos-1;
	if(!m_sections)
	{
		return false;
	}
	m_section_length = distancia/m_sections;
	m_interval_time = duracion/m_sections;

//que el tiempo total de para hacer todas las fotos
	if( m_total_time < (m_section_length*m_sections + fotos*m_take_time) )
	{
		return false;
	}
//que de tiempo a hacer la foto y a moverse antes de empezar la proxima
	if( m_interval_time < (m_section_length + m_take_time) )
	{
		return false;
	}
m_time=0;
m_count=0;
	return true;	
} 

void cart::print_config()
{
	std::cout<<"m_sections "	<<m_sections<<std::endl;
	std::cout<<"m_total "		<<m_total_time<<std::endl;
	std::cout<<"m_take_time "	<<m_take_time<<std::endl;
	std::cout<<"m_interval_time "	<<m_interval_time<<std::endl;
	std::cout<<"m_section_length "	<<m_section_length<<std::endl;
	std::cout<<std::endl;
}

void cart::print_status()
{
	std::cout<<"m_pos "	<<m_pos<<std::endl;
	std::cout<<std::endl;
}

void cart::program()
{
	if(m_time > m_total_time+m_take_time)
	{
		std::cout<<"Tiempo"<<std::endl;
		m_prog=PROG_FINISH;
		return;
	}

	if( !(m_time%m_interval_time))
	{
		m_count=m_take_time;
		m_action = ACTION_TAKING;	
	} 	

	switch(m_action)
	{
		case ACTION_NONE:
			break;
		case ACTION_TAKING:
			--m_count;
			if(!m_count)
			{
				std::cout<<"Taken"<<std::endl;
				m_count = m_section_length;
				m_action = ACTION_MOVING;
			}
			break;
		case ACTION_MOVING:
			--m_count;
			move(DIR::END);
			if(!m_count)
			{
				std::cout<<"Moved"<<std::endl;
				m_action = ACTION_NONE;
			}
			break;

	}
//std::cout<<m_time<<std::endl;
++m_time;
}

bool cart::wait()
{

	return false;
}

bool cart::take()
{

	if( !m_count )
	{
		digitalWrite((int)GPIO::OBT,1);
		m_count = m_take_time+1;
	}
	--m_count;

	if( !m_count )
	{
		digitalWrite((int)GPIO::OBT,0);
		return true;
	}
	return false;

}

bool cart::time_step()
{
	if(m_time>=m_total_time)
		return true;
	++m_time;
	return false;

}

bool cart::move(DIR dir)
{
	if(!digitalRead((int)GPIO::ENDSWITCH) && (dir == DIR::END))
	{
		std::cout<<"Tamper final"<<std::endl;
		m_prog = PROG_FINISH;
		return false;
	}	
	if(!digitalRead((int)GPIO::STARTSWITCH) && (dir == DIR::START))
	{
		std::cout<<"Tamper init"<<std::endl;
		m_prog = PROG_FINISH;
		m_pos=0;
		return false;
	}	
	
	//move a step
	int a,b,c,d;
	switch(m_stepper_pos%4)
	{
		case 0:
			a=1;
			b=0;
			c=1;
			d=0;
			break;
		case 1:
			a=1;
			b=0;
			c=0;
			d=1;
			break;
		case 2:
			a=0;
			b=1;
			c=0;
			d=1;
			break;
		case 3:
			a=0;
			b=1;
			c=1;
			d=0;
			break;
	}

	digitalWrite((int)GPIO::AA,a);
	digitalWrite((int)GPIO::AB,b);
	digitalWrite((int)GPIO::BA,c);
	digitalWrite((int)GPIO::BB,d);
	
if(dir == DIR::END){
++m_pos;
	++m_stepper_pos;
}
else{
	--m_stepper_pos;
--m_pos;
}

	return true;
}
