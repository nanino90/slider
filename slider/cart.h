#ifndef CART_H
#define CART_H
#include <stdio.h>
#include <stdint.h>
#include <iostream>
#include "status.h"

enum class GPIO:int{
	OUT =0,
	IN  =1,
	OBT =0,
	BA  =22,
	BB  =23,
	AA  =24,
	AB  =25,
	ENDSWITCH=30,
	STARTSWITCH=21

};

class cart {

	public:
		cart(void);
		~cart(void);
		uint16_t get_pos(void);
		bool set_program(uint32_t obt,uint32_t duracion,uint16_t fotos, uint16_t distancia);
		void print_config();
		void print_status();
		bool time_step();
		void program();
		bool move(DIR dir, uint16_t);
		void reset_cont();
		void take(uint32_t tiempo);
		PROG m_prog;
		ACTION m_action;

	private:
		bool move();
		void release();
		uint32_t m_total_time;
		uint32_t m_take_time;
		uint32_t m_interval_time;
		uint16_t m_sections;
		uint16_t m_section_length;
		uint32_t m_time;
		uint32_t m_pos;
		uint32_t m_count;
		uint8_t	m_stepper_pos;
};

#endif
