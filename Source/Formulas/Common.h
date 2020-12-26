#pragma once

#include <complex>
#include <map>
#include <string>

struct VariablesList
{
    std::map<std::string, std::complex<float>*> Vars;
};
