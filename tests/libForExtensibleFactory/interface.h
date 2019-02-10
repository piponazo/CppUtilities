#pragma once

#include "ExtTest_export.h"

class EXTTEST_EXPORT Interface
{
    public:
        virtual ~Interface() {}

        virtual int foo() = 0;
};

EXTTEST_EXPORT void registerImplementations();
