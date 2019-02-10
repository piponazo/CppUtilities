#pragma once

#include <cstdlib>

/// Literals which allows to compare memory sizes in Gigabytes.
/// \param[in] gygabytes Number of gigabytes
/// \return The equivalent in bytes of \b gygabytes
/// \todo Move this operator to a CORE library
constexpr unsigned long long operator"" _GB ( unsigned long long gygabytes )
{
    return gygabytes*1'073'741'824;
}
