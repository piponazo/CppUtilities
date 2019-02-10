#include <catch2/catch.hpp>

#include "../libForExtensibleFactory/interface.h"
#include <utilities/ExtensibleFactory.h>

TEST_CASE("ExtensibleFactory shouldRegisterImplementations")
{
    registerImplementations();
    REQUIRE(ut::ExtensibleFactory<Interface>::instance().registeredTypes() == 2);
}

TEST_CASE("ExtensibleFactory shouldCreateImplementation")
{
    REQUIRE(ut::ExtensibleFactory<Interface>::instance().registeredTypes() == 2);
    auto impl = ut::ExtensibleFactory<Interface>::instance().create("Type1");
    REQUIRE(impl);
}
