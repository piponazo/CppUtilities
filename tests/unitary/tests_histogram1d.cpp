#include <utilities/histogram1D.h>

#include <catch2/catch.hpp>

#include <memory>
#include <iostream>

using ut::Histogram1D;
using namespace std;

void checkBinRange(const Histogram1D &histo, const std::uint32_t bins)
{
        for (std::uint32_t i = 0; i < bins; ++i) {
                const std::pair<double, double> range = histo.getRange(i);
                const double inc = histo.getRangeDistance() * i;
                const double first = histo.getMin() + inc;
                const double second = first + histo.getRangeDistance();
                REQUIRE(range.first == first);
                REQUIRE(range.second == second);
        }
}

TEST_CASE("Histogram1D shouldCreateAndDestroyInstanceCorrectly")
{
        Histogram1D histo(0., 10., 2);
        REQUIRE(histo.getMin() == 0.);
        REQUIRE(histo.getMax() == 10.);
        REQUIRE(histo.getBins() == 2);
}

TEST_CASE("Histogram shouldObtainCorrectRangeDistance")
{
        std::unique_ptr<Histogram1D> histo = std::unique_ptr<Histogram1D>(new Histogram1D(0., 10., 2));
        REQUIRE(histo->getRangeDistance() == 5.);

        histo = std::unique_ptr<Histogram1D>(new Histogram1D(-5., 5., 2));
        REQUIRE(histo->getRangeDistance() == 5.);

        histo = std::unique_ptr<Histogram1D>(new Histogram1D(-10., 20., 2));
        REQUIRE(histo->getRangeDistance() == 15.); // 30 / 2

        histo = std::unique_ptr<Histogram1D>(new Histogram1D(-5.5, 10., 8));
        REQUIRE(histo->getRangeDistance() == 1.9375);
}

TEST_CASE("Histogram shouldObtainCorrectBinRanges")
{
        Histogram1D histo(0., 10., 5);
        checkBinRange(histo, 5);
        Histogram1D histo2(-5.5, 10., 8);
        checkBinRange(histo2, 8);
}

TEST_CASE("Histogram shouldNotAddValuesOutOfRange")
{
        Histogram1D histo(0., 5., 5);
        REQUIRE_THROWS_AS(histo.addValue(-0.0001), std::out_of_range);
        REQUIRE_THROWS_AS(histo.addValue(5.0001), std::out_of_range);
}

TEST_CASE("Histogram shouldAddValuesAndCountThem")
{
        Histogram1D histo(0., 5., 5);
        for (std::uint32_t i = 0; i < 100; ++i) {
                histo.addValue(i % 5);
        }
        REQUIRE(histo.nValues() == 100);
}

TEST_CASE("Histogram shouldAddSameNumberOfValuesForEachBinAndCheckPercentages")
{
        Histogram1D histo(0., 5., 5);
        for (int i = 0; i < 100; ++i) {
                histo.addValue(i % 5); // each bin will acumulate 20 values
        }
        for (int i = 0; i < 5; ++i) {
                REQUIRE(histo.getPercentage(i) == 0.2);
                REQUIRE(histo.getPercentageBelow(i) == 0.2 * (i+1));
                REQUIRE(histo.getPercentageAbove(i) == 1 - (0.2 * i));
        }
}

TEST_CASE("Histogram shouldThrowExceptionWhenTryingToAccessANonExistingBin")
{
        Histogram1D histo(0., 10., 5);
        REQUIRE_THROWS_AS(histo.getPercentage(6), std::out_of_range);
        REQUIRE_THROWS_AS(histo.getPercentageBelow(6), std::out_of_range);
        REQUIRE_THROWS_AS(histo.getPercentageAbove(6), std::out_of_range);
}
