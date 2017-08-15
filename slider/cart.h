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
		
		void set_run1(uint32_t obt,uint32_t duracion,uint16_t fotos, uint16_t distancia);
void print_config();
void print_status();
		uint16_t m_interval;	//distancia en steps

		uint32_t m_velocidad;	//ticks por step
		uint16_t m_max_distancia;	//distancia en steps
		uint16_t m_pos;		//position en steps
		uint16_t m_distancia;	//distancia en steps
	private:
		uint32_t m_obt;		//ticks de duracion de obturacion
		uint32_t m_duracion;	//ticks de duracion total
		uint16_t m_fotos;	//numero de fotos
		uint8_t	 m_move_time;
};

#endif
