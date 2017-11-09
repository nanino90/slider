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
	m_prog(PROG_INIT),
	m_action(ACTION_NONE),
	m_stepper_pos(0)
{
	pinMode((int)GPIO::OBT,OUTPUT);
	pinMode((int)GPIO::AA,OUTPUT);
	pinMode((int)GPIO::AB,OUTPUT);
	pinMode((int)GPIO::BA,OUTPUT);
	pinMode((int)GPIO::BB,OUTPUT);

}

cart::~cart(void)
{
release();
}

void cart::release()
{
	digitalWrite((int)GPIO::AA,0);
	digitalWrite((int)GPIO::AB,0);
	digitalWrite((int)GPIO::BA,0);
	digitalWrite((int)GPIO::BB,0);
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
	std::cout<<"m_time "	<<m_time<<std::endl;
	std::cout<<std::endl;
}

void cart::program()
{
	if(m_time > m_total_time+m_take_time)
	{
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
			digitalWrite((int)GPIO::OBT,1);
			if(!m_count)
			{
				m_count = m_section_length;
				m_action = ACTION_MOVING;
				digitalWrite((int)GPIO::OBT,0);
			}
			break;
		case ACTION_MOVING:
			--m_count;
			move(DIR::END, 1);
			if(!m_count)
			{
				release();
				m_action = ACTION_NONE;
			}
			break;

	}
	++m_time;
}

void cart::reset_cont()
{
	m_count=0;
	return;
}

void cart::take(uint32_t tiempo)
{
	digitalWrite((int)GPIO::OBT,1);
	std::cout<<"taking"<<std::endl;
	if( m_count >= tiempo )
	{
		std::cout<<"end taking"<<std::endl;
		digitalWrite((int)GPIO::OBT,0);
		m_prog=PROG_FINISH;
		return;
	}
	++m_count;
}

bool cart::time_step()
{
	if(m_time>=m_total_time)
		return true;
	++m_time;
	return false;

}

bool cart::move(DIR dir, uint16_t speed)
{
	if(m_time%speed)
	{
		++m_time;
		return true;
	}
	if(!digitalRead((int)GPIO::ENDSWITCH) && (dir == DIR::END))
	{
		m_prog = PROG_FINISH;
		return false;
	}	
	if(!digitalRead((int)GPIO::STARTSWITCH) && (dir == DIR::START))
	{
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
	++m_time;
	return true;
}

