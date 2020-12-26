#pragma once

#include <complex>
#include <map>
#include <string>

#include "Types.h"

struct VariablesList
{
    std::map<std::string, Complex*> Vars;
};
