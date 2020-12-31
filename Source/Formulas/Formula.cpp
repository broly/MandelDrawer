#include "Formula.h"

#include "../Types.h"

using namespace Mandel;

Formula::Formula()
{
    InputFormula = "";
    FormulaParser.SetInput("");
    SetVariables(nullptr);
    bHasDirtyFormula = false;
}

Formula::Formula(std::string InInputFormula, std::shared_ptr<VariablesList> Vars)
{
    SetFormula(InInputFormula);
    SetVariables(Vars);
}

void Formula::SetFormula(std::string InInputFormula)
{
    InputFormula = InInputFormula;
    bHasDirtyFormula = true;
    FormulaParser.SetInput(InputFormula.c_str());
}

void Formula::SetVariables(std::shared_ptr<VariablesList> Vars)
{
    FormulaParser.SetVariables(Vars);
}

void Formula::SetVariable(std::string VarName, Complex* Var)
{
    FormulaParser.SetVariable(VarName, Var);
}

void Formula::Parse()
{
    bHasDirtyFormula = false;
    FormulaExpression = FormulaParser.Parse();
}

Complex Formula::EvaluateOnFly()
{
    if (InputFormula != "")
    {
        if (bHasDirtyFormula || !FormulaExpression)
        {
            Parse();
        }

        bHasError = GetError(ErrorReason);

        if (!bHasError)
            return FormulaExpression->Evaluate();
    }
    return {};
}

bool Formula::GetError(std::string& Reason)
{
    if (FormulaExpression->HasError())
    {
        Reason = FormulaExpression->GetErrorReason();
        return true;
    }
    return false;
}

