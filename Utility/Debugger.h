#pragma once

#include <assert.h>

#define D_ASSERT(expression,fmt,...) { if(!(expression)){ assert(expression) std::cout << fmt << endl; } }

class Debugger
{
};

