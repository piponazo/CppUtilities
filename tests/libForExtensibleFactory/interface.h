#pragma once

#include "ExtTest_export.h"

class EXPORT Interface
{
	public:
		virtual ~Interface() {}

		virtual int foo() = 0;
};

void EXPORT registerImplementations();
