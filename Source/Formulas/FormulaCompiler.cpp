#include "FormulaCompiler.h"


#include <iostream>
#include <locale>

using namespace Mandel;

FormulaCompiler::FormulaCompiler(const char* InFormula)
{
    Formula = InFormula;
    CompiledBytecode = {};
}

void FormulaCompiler::Compile()
{
    if (CompiledBytecode.empty())
    {

    }
}
