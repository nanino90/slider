#include "cart.h"

cart::cart(void):
	m_pos(0),
	m_obt(0),	      
	m_velocidad(0), 
	m_duracion(0),  
	m_fotos(0),     
	m_distancia(0), 
	m_max_distancia(10000),
	m_move_time(0),
	m_interval(0) 
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
	uint32_t interval;
	interval = (uint32_t)(duracion/(uint32_t)fotos); 

	if(interval <= obt+m_move_time)
	{
		std::cout<<"Valores no compatibles"<<std::endl;
		return;	
	}
	if(distancia > m_pos + m_max_distancia)
	{
		std::cout<<"Distancia erronea"<<std::endl;
		return;
	}

	m_interval	= interval;
	m_velocidad	= duracion / distancia;
	m_obt 		= obt;
	m_duracion 	= duracion;
	m_fotos		= fotos;
	m_distancia	= distancia;

	return;	
} 

void cart::print_config()
{
	std::cout<<"Velocidad "<<m_velocidad<<std::endl;
	std::cout<<"Obturacion "<<m_obt<<std::endl;
	std::cout<<"Distancia "<<m_distancia<<std::endl;
	std::cout<<"Duracion "<<m_duracion<<std::endl;
	std::cout<<"Fotos "<<m_fotos<<std::endl;
	std::cout<<"Interval "<<m_interval<<std::endl;
	std::cout<<std::endl;
}

void cart::print_status()
{
	std::cout<<"Fotos "<<m_fotos<<std::endl;
	
}


