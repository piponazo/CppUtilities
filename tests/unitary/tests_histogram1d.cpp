#include "histogram1D.h"
#include <gtest/gtest.h>

TEST(Histogram1DTest, shouldCreateAndDestroyInstanceCorrectly)
{
	ut::Histogram1D histo(0., 10., 2);
}
