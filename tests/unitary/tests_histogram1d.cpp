#include "histogram1D.h"
#include <gtest/gtest.h>
#include <memory>

using ut::Histogram1D;

void checkBinRange(const Histogram1D &histo, const std::uint32_t bins)
{
	for (std::uint32_t i = 0; i < bins; ++i) {
		const std::pair<double, double> range = histo.getRange(i);
		const double inc = histo.getRangeDistance() * i;
		const double first = histo.getMin() + inc;
		const double second = first + histo.getRangeDistance();
		ASSERT_DOUBLE_EQ(range.first, first);
		ASSERT_DOUBLE_EQ(range.second, second);
	}
}

TEST(Histogram1DTest, shouldCreateAndDestroyInstanceCorrectly)
{
	Histogram1D histo(0., 10., 2);
	ASSERT_DOUBLE_EQ(histo.getMin(), 0.);
	ASSERT_DOUBLE_EQ(histo.getMax(), 10.);
	ASSERT_EQ(histo.getBins(), 2);
}

TEST(Histogram1DTest, shouldObtainCorrectRangeDistance)
{
	std::unique_ptr<Histogram1D> histo = std::unique_ptr<Histogram1D>(new Histogram1D(0., 10., 2));
	ASSERT_DOUBLE_EQ(histo->getRangeDistance(), 5.);

	histo = std::unique_ptr<Histogram1D>(new Histogram1D(-5., 5., 2));
	ASSERT_DOUBLE_EQ(histo->getRangeDistance(), 5.);

	histo = std::unique_ptr<Histogram1D>(new Histogram1D(-10., 20., 2));
	ASSERT_DOUBLE_EQ(histo->getRangeDistance(), 15.);

	histo = std::unique_ptr<Histogram1D>(new Histogram1D(-5.5, 10., 8));
	ASSERT_DOUBLE_EQ(histo->getRangeDistance(), 1.9375);
}

TEST(Histogram1DTest, shouldObtainCorrectBinRanges)
{
	std::unique_ptr<Histogram1D> histo = std::unique_ptr<Histogram1D>(new Histogram1D(0., 10., 5));
	checkBinRange(*histo, 5);
	histo = std::unique_ptr<Histogram1D>(new Histogram1D(-5.5, 10., 8));
	checkBinRange(*histo, 8);
}

TEST(Histogram1DTest, shouldNotAddValuesOutOfRange)
{
	std::unique_ptr<Histogram1D> histo = std::unique_ptr<Histogram1D>(new Histogram1D(0., 5., 5));
	ASSERT_THROW(histo->addValue(-0.0001), std::out_of_range);
	ASSERT_THROW(histo->addValue(5.0001), std::out_of_range);
}

TEST(Histogram1DTest, shouldAddValuesAndCountThem)
{
	std::unique_ptr<Histogram1D> histo = std::unique_ptr<Histogram1D>(new Histogram1D(0., 5., 5));
	for (std::uint32_t i = 0; i < 100; ++i) {
		histo->addValue(i % 5);
	}
	ASSERT_EQ(histo->nValues(), 100);
}

TEST(Histogram1DTest, shouldAddSameNumberOfValuesForEachBinAndCheckPercentages)
{
	std::unique_ptr<Histogram1D> histo = std::unique_ptr<Histogram1D>(new Histogram1D(0., 5., 5));
	for (std::uint32_t i = 0; i < 100; ++i) {
		histo->addValue(i % 5);
	}
	for (std::uint32_t i = 0; i < 5; ++i) {
		ASSERT_DOUBLE_EQ(histo->getPercentage(i), 0.2);
		ASSERT_DOUBLE_EQ(histo->getPercentageBelow(i), 0.2 * (i+1));
		ASSERT_DOUBLE_EQ(histo->getPercentageAbove(i), 1 - (0.2 * i));
	}
}

TEST(Histogram1DTest, shouldThrowExceptionWhenTryingToAccessANonExistingBin)
{
	std::unique_ptr<Histogram1D> histo = std::unique_ptr<Histogram1D>(new Histogram1D(0., 10., 5));
	ASSERT_THROW(histo->getPercentage(6), std::out_of_range);
	ASSERT_THROW(histo->getPercentageBelow(6), std::out_of_range);
	ASSERT_THROW(histo->getPercentageAbove(6), std::out_of_range);
}
