#include "histogram1D.h"

namespace ut
{

Histogram1D::Histogram1D(const double min, const double max, const std::uint32_t bins) :
	_min (min)
  , _max (max)
  , _bins (bins)
{
}

}
