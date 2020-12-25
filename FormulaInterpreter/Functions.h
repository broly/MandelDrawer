#pragma once

#include <complex>
#include <cmath>
#include <map>
#include <vector>

/** Helper macro to use each pristine function */
#define BUILTIN_SINGLE_ARG_FUNC(func_name) \
    static std::complex<float> func_name(std::complex<float> x) { return std::func_name(x); }

/**
 * All single-arg functions
 */
struct _SingleFunctions
{
    // ********* PRISTINE ***********
    BUILTIN_SINGLE_ARG_FUNC(sin);
    BUILTIN_SINGLE_ARG_FUNC(cos);
    BUILTIN_SINGLE_ARG_FUNC(tan);
    BUILTIN_SINGLE_ARG_FUNC(sqrt);
    BUILTIN_SINGLE_ARG_FUNC(abs);
    // ******* END PRISTINE ********

    // ...
};

/** Type of single-arg function */
using SingleFunctionType = std::complex<float>(*)(std::complex<float> x);

/**
 * List of all single-arg functions
 * TODO: register each function here!
 */
static std::vector<SingleFunctionType> SingleFunctions = {
    _SingleFunctions::sin,
    _SingleFunctions::cos,
    _SingleFunctions::tan,
    _SingleFunctions::sqrt,
    _SingleFunctions::abs,
};

