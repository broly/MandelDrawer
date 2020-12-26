#include "Formula.h"

Formula::Formula(std::string InInputFormula, VariablesList Vars)
{
    InputFormula = InInputFormula;
    FormulaParser.SetInput(InputFormula.c_str());
    SetVariables(Vars);
    bHasDirtyFormula = true;
}

void Formula::SetVariables(VariablesList Vars)
{
    FormulaParser.SetVariables(Vars);
}

std::complex<float> Formula::EvaluateOnFly()
{
    if (bHasDirtyFormula || !FormulaExpression)
    {
        bHasDirtyFormula = false;
        FormulaExpression = FormulaParser.Parse();
    }

    return FormulaExpression->Evaluate();
}

