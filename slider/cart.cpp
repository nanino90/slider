#include "cart.h"

cart::cart(void):
	m_pos(0)
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

