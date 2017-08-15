#ifndef CONTEXT_H
#define CONTEXT_H

#include <stdio.h>
#include <stdint.h>
#include <iostream>
#include "status.h"

typedef struct{
	STATUS status;

} context;


void context_construct(context* p);

context* get_context();

#endif
