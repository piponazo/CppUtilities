#include "implementation2.h"

int Impl2::foo()
{
	return 2;
}

std::unique_ptr<Interface> Impl2::create()
{
	return std::unique_ptr<Interface>(new Impl2);
}
