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

Formula::~Formula()
{
    // std::cout << "~Formula" << std::endl;
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
    Interpreter.SetVars(Vars);
}

void Formula::SetVariable(std::string VarName, Complex* Var)
{
    FormulaParser.SetVariable(VarName, Var);
    Interpreter.SetVariable(VarName, Var);
}

void Formula::Compile()
{
    if (InputFormula != "")
    {
        Parse();

        bHasError = GetError(ErrorReason);
        if (bHasError)
        {
            std::cout << "Could not compile formula " << InputFormula << ", because " << ErrorReason;
        }

        if (!bHasError)
        {
            CompilationInfo Info;
            uint16 LastSlot = 0;
            FormulaExpression->Compile(Info, LastSlot);
            Interpreter.SetCompiledData(Info, LastSlot);
        }
    }
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

Complex Formula::EvaluateCompiled()
{
    return Interpreter.Execute();
    
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

