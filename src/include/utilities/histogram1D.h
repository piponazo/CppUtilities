#pragma once

#include "utilities_export.h"

#include <cstdint>
#include <utility>
#include <memory>

namespace ut
{

/// Histogram of 1 dimension for gather statistics of the distribution of certain measures.
class UTILITIES_EXPORT Histogram1D
{
public:
    /// Constructor
    /// \param min [in] Minimum value to be considered by the histogram
    /// \param max [in] Maximum value to be considered by the histogram
    /// \param bins[in] Number of bins in which that the histogram will use.
    Histogram1D(const double min, const double max, const std::uint32_t bins);
    ~Histogram1D();

    /////////////////
    // Accessors
    /////////////////

    double getMin() const;

    double getMax() const;

    std::uint32_t getBins() const;

    std::uint32_t nValues() const;

    /// \brief Gets the distance between the minimum and maximum value for each bin. (Common for all the bins)
    double getRangeDistance() const;

    /// \brief Gets the range (minimum and maximum value) of a specific bin.
    std::pair<double, double> getRange(const std::uint32_t bin) const;

    /// \brief Gets the percentage of values accumulated below the \b bin specified.
    double getPercentageBelow(const std::uint32_t bin) const;

    /// \brief Gets the percentage of values accumulated from the \b bin specified and the bins above it.
    double getPercentageAbove(const std::uint32_t bin) const;

    /// \brief Gets the percentage of values accumulated in the \b bin specified.
    double getPercentage(const std::uint32_t bin) const;

    /////////////////
    // Mutators
    /////////////////

    /// \brief Adds a new value to the distribution.
    void addValue(const double val);

    /// \brief Reset all the values gathered until now.
    void reset();

private:
    void checkBin(const std::uint32_t bin) const;
    double accFromTo(const std::uint32_t from, const std::uint32_t to, const std::uint32_t bin) const;

    struct Pimpl;
    std::unique_ptr<Pimpl> p;
};

}
