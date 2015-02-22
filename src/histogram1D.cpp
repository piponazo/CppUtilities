#include "histogram1D.h"
#include "macros.h"

#include <vector>
#include <stdexcept>

namespace ut
{

struct Histogram1D::Pimpl
{
	Pimpl(const double min_, const double max_, const std::uint32_t bins_) :
		min (min_)
	  , max (max_)
	  , bins (bins_)
	  , acc(bins, 0)
	{
	}

	const double        min;
	const double        max;
	const std::uint32_t bins;
	std::vector<double> acc;
	double              nValues;
};

Histogram1D::Histogram1D(const double min, const double max, const std::uint32_t bins) : p(new Pimpl(min, max, bins))
{
}

Histogram1D::~Histogram1D()
{
}

double Histogram1D::getMin() const
{
	return p->min;
}

double Histogram1D::getMax() const
{
	return p->max;
}

std::uint32_t Histogram1D::getBins() const
{
	return p->bins;
}

std::uint32_t Histogram1D::nValues() const
{
	return static_cast<std::uint32_t>(p->nValues);
}

double Histogram1D::getRangeDistance() const
{
	return (p->max - p->min) / static_cast<double>(p->bins);
}

std::pair<double, double> Histogram1D::getRange(const std::uint32_t bin) const
{
	return std::make_pair(p->min + getRangeDistance() * bin,
						  p->min + getRangeDistance() * (bin + 1) );
}

double Histogram1D::getPercentageBelow(const std::uint32_t bin) const
{
	checkBin(bin);
	return accFromTo(0, bin, bin);
}

double Histogram1D::getPercentageAbove(const std::uint32_t bin) const
{
	checkBin(bin);
	return accFromTo(bin, p->bins-1, bin);
}

double Histogram1D::getPercentage(const std::uint32_t bin) const
{
	return p->acc.at(bin) / p->nValues;
}

void Histogram1D::addValue(const double val)
{
	if (val < p->min || val > p->max) {
		throw std::out_of_range("Value if out of the range of the histogram");
	}

	std::uint32_t bin = 0;
	while ( (p->min + getRangeDistance() * (bin + 1)) <= val ) {
		bin++;
	}

	p->acc[bin]+=1.;
	p->nValues+=1.;
}

void Histogram1D::checkBin(const std::uint32_t bin) const
{
	if (bin > p->bins) {
		throw std::out_of_range("Trying to access to non existing bin");
	}
}

double Histogram1D::accFromTo(const std::uint32_t from, const std::uint32_t to, const std::uint32_t bin) const
{
	double acc = 0.;
	for (std::uint32_t i = from; i <= to; ++i) {
		acc += p->acc.at(bin) / p->nValues;
	}
	return acc;
}

}
