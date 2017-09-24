#ifndef STATUS_H
#define STATUS_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string>

typedef enum{
	STATUS_INIT =0x10,
	STATUS_IDLE,
	STATUS_RUNNING,
	STATUS_FINISHED,
	
	STATUS_SIZE
}STATUS;

typedef enum{
	MODE_PROGRAM = 0x20,
	MODE_TO_END,
	MODE_TO_START,
	MODE_MANUAL,

	MODE_SIZE
}MODE;

typedef enum{
	PROG_INIT =0x10,
	PROG_CONFIGURED  ,
	PROG_RUNNING  ,
	PROG_MOVE_T0_END  ,
	PROG_FINISH ,

	PROG_SIZE
}PROG;

enum class DIR{
	END,
	START
};

#endif
