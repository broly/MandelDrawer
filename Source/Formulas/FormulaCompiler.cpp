#include "FormulaCompiler.h"


#include <iostream>
#include <locale>


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
