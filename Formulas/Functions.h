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
    static std::complex<float> negate(std::complex<float> x) { return -x; };
    static std::complex<float> positive(std::complex<float> x) { return +x; };
    // ...
};

/** Type of single-arg function */
using SingleFunctionType = std::complex<float>(*)(std::complex<float> x);

struct SingleFunctionInfo
{
    std::string FunctionName;
    SingleFunctionType Func;
};

/**
 * List of all single-arg functions
 * TODO: register each function here!
 */
static std::vector<SingleFunctionInfo> SingleFunctions = {
    {"sin", _SingleFunctions::sin},
    {"cos", _SingleFunctions::cos},
    {"tan", _SingleFunctions::tan},
    {"sqrt", _SingleFunctions::sqrt},
    {"abs", _SingleFunctions::abs},
    {"-", _SingleFunctions::negate},
    {"+", _SingleFunctions::positive},
};

