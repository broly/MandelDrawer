#pragma once

#include <complex>
#include <map>
#include <string>

#include "Types.h"

namespace Mandel
{
    struct VariablesList
    {
        std::map<std::string, Complex*> Vars;
    };
}