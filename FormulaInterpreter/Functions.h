#pragma once

#include <complex>
#include <cmath>
#include <map>
#include <vector>


#define BUILTIN_SINGLE_ARG_FUNC(func_name) \
    static std::complex<float> func_name(std::complex<float> x) { return std::func_name(x); }


struct _SingleFunctions
{
    BUILTIN_SINGLE_ARG_FUNC(sin);
    BUILTIN_SINGLE_ARG_FUNC(cos);
    BUILTIN_SINGLE_ARG_FUNC(tan);
    BUILTIN_SINGLE_ARG_FUNC(sqrt);
    BUILTIN_SINGLE_ARG_FUNC(abs);
};

using SingleFunctionType = std::complex<float>(*)(std::complex<float> x);


static std::vector<SingleFunctionType> SingleFunctions = {
    _SingleFunctions::sin,
    _SingleFunctions::cos,
    _SingleFunctions::tan,
    _SingleFunctions::sqrt,
    _SingleFunctions::abs,
};

