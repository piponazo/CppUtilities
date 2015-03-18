#include "implementation1.h"

int Impl1::foo()
{
	return 1;
}

std::unique_ptr<Interface> Impl1::create()
{
	return std::unique_ptr<Interface>(new Impl1);
}
