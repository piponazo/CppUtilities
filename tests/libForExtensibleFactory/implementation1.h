#pragma once

#include "interface.h"
#include <memory>

class Impl1 : public Interface
{
	public:
		int foo() override;
		static std::unique_ptr<Interface> create();
};
