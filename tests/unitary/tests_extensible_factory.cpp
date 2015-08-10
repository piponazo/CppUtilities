#include "../libForExtensibleFactory/interface.h"
#include "ExtensibleFactory.h"

#include <gtest/gtest.h>

TEST(ExtensibleFactoryTests, shouldRegisterImplementations)
{
	registerImplementations();
	ASSERT_EQ(ut::ExtensibleFactory<Interface>::instance().registeredTypes(), 2);
}

TEST(ExtensibleFactoryTests, shouldCreateImplementation)
{
	ASSERT_EQ(ut::ExtensibleFactory<Interface>::instance().registeredTypes(), 2);
	auto impl = ut::ExtensibleFactory<Interface>::instance().create("Type1");
	ASSERT_NE (impl.get(), nullptr);
}
