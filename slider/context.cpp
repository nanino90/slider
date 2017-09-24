#include "context.h"

context con;

void context_construct(context* p)
{
	p->status=STATUS_INIT;
	p->mode= MODE_SIZE;
}

context* get_context()
{
	return &con;
}

