#ifndef PARSE
#define PARSE

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <iostream>
#include <string>
#include <chrono>
#include <thread>
#include "parse.h"
#include "context.h"
#include "status.h"

bool parse(context* cont, int argc, char** argv);

#endif
