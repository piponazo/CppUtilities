#include "interface.h"
#include "ExtensibleFactory.h"

#include "implementation1.h"
#include "implementation2.h"

void registerImplementations()
{
        auto & factory = ut::ExtensibleFactory<Interface>::instance();
	factory.registerType("Type1", Impl1::create);
	factory.registerType("Type2", Impl2::create);
}
