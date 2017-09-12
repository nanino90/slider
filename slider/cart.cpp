#include "cart.h"
#include <wiringPi.h>

cart::cart(void):
	m_total_time(0),
	m_take_time(0),
	m_move_time(0),
	m_sections(0),
	m_section_steps(0),
	m_time(0),
	m_pos(0),
	m_count(0),
	m_stepper_pos(0)
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
	std::cout<<"Cart erased"<<std::endl;
}

uint16_t cart::get_pos(void)
{
	return m_pos;
}

void cart::set_run1(uint32_t obt,uint32_t duracion,uint16_t fotos, uint16_t distancia)
{
	m_total_time=duracion;

	m_take_time = obt;

	m_sections = fotos-1;

	m_move_time = (duracion-((uint32_t)fotos*obt))/m_sections;

	m_section_steps = distancia/m_sections;

	return;	
} 

void cart::print_config()
{
	std::cout<<"m_take_time "	<<m_take_time<<std::endl;
	std::cout<<"m_sections "	<<m_sections<<std::endl;
	std::cout<<"m_move_time "	<<m_move_time<<std::endl;
	std::cout<<"m_section_steps "	<<m_section_steps<<std::endl;
	std::cout<<std::endl;
}

void cart::print_status()
{

}

bool cart::validate_config()
{
	//Validate configuration
	//Constrain1: moving speed

	if( (uint32_t)m_section_steps >= m_move_time )
		return 1;

	return 0;
}

bool cart::move()
{
	if(m_total_time%100)
		return false;

	//move a step
	int a;
	int b;
	int c;
	int d;
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
default:
	std::cout<<"DEFUALT"<<std::endl;
break;
	}

	digitalWrite((int)GPIO::AA,a);
	digitalWrite((int)GPIO::AB,b);
	digitalWrite((int)GPIO::BA,c);
	digitalWrite((int)GPIO::BB,d);
	
	++m_stepper_pos;

	++m_pos;

	if( !m_count )
		m_count = (uint32_t)m_section_steps+1;

	--m_count;

	if(!digitalRead((int)GPIO::ENDSWITCH))
	{
		std::cout<<"Tamper final"<<std::endl;
		m_time = m_total_time;
		return false;
	}	

	if( !m_count )
		return true;
	
	return false;
}

bool cart::wait()
{
	if( !m_count )
		m_count = m_move_time - (uint32_t)m_section_steps+1;

	--m_count;

	if( !m_count )
		return true;

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
