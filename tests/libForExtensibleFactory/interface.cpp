#include "interface.h"
#include "ExtensibleFactory.h"

#include "implementation1.h"
#include "implementation2.h"

void registerImplementations()
{
	ut::ExtensibleFactory<Interface>::registerType("Type1", Impl1::create);
	ut::ExtensibleFactory<Interface>::registerType("Type2", Impl2::create);
}
