#ifndef CART_H
#define CART_H
#include <stdio.h>
#include <stdint.h>
#include <iostream>

class cart {

	public:
		cart(void);
		
		~cart(void);
	
		uint16_t get_pos(void);

	private:
		uint16_t m_pos;



};

#endif
