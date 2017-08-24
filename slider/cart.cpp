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
		m_count(0)
{
	std::cout<<"Cart init"<<std::endl;
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
	//move a step
	++m_pos;

	if( !m_count )
		m_count = (uint32_t)m_section_steps;

	--m_count;

	if( !m_count )
		return true;
	return false;
}

bool cart::wait()
{
	if( !m_count )
		m_count = m_move_time - (uint32_t)m_section_steps;

	--m_count;

	if( !m_count )
		return true;

	return false;
}

bool cart::take()
{
	if( !m_count )
		m_count = m_take_time;

	--m_count;

	if( !m_count )
		return true;
	return false;

}

void cart::time_step()
{
	++m_time;
}
