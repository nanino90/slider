#ifndef CART_H
#define CART_H
#include <stdio.h>
#include <stdint.h>
#include <iostream>

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
		void set_run1(uint32_t obt,uint32_t duracion,uint16_t fotos, uint16_t distancia);
		void print_config();
		void print_status();
		bool validate_config();
		bool time_step();
		bool move();
		bool move_to_end(int dir);
		bool wait();
		bool take();

	private:
		uint32_t m_total_time;
		uint32_t m_take_time;
		uint32_t m_move_time;
		uint16_t m_sections;
		uint16_t m_section_steps;
		uint32_t m_time;
		uint32_t m_pos;
		uint32_t m_count;
		uint8_t	m_stepper_pos;
};

#endif
