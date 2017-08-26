#ifndef CONTEXT_H
#define CONTEXT_H

#include <stdio.h>
#include <stdint.h>
#include <iostream>
#include "status.h"
#include "cart.h"

typedef struct{
	STATUS status;
	cart car;

} context;


void context_construct(context* p);

context* get_context();

#endif
