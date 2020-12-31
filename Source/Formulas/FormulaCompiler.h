#pragma once
#include <string>
#include <vector>

#include "../Types.h"

namespace Mandel
{
    class FormulaCompiler
    {
    public:
        FormulaCompiler(const char* InFormula);

        void Compile();

        const char* Formula;
    
        std::vector<uint8> CompiledBytecode;

    };
}