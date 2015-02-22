#pragma once

#include "CppUtilities_export.h"
#include <cstdint>

namespace ut
{

/// Histogram of 1 dimension for gather statistics of the distribution of certain measures.
class EXPORT Histogram1D
{
public:
	Histogram1D(const double min, const double max, const std::uint32_t bins);

private:
	const double        _min;
	const double        _max;
	const std::uint32_t _bins;
};

}
