#pragma once

#include "ExtTest_export.h"

class UTILITIES_EXPORT Interface
{
	public:
		virtual ~Interface() {}

		virtual int foo() = 0;
};

void UTILITIES_EXPORT registerImplementations();
