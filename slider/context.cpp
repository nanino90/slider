#include "context.h"

context con;

void context_construct(context* p)
{
	p=&con;
	p->status=STATUS_INIT;
}

context* get_context()
{
	return &con;
}

