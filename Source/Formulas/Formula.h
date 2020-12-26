#pragma once
#include <string>

#include "Parser.h"

class Formula
{
public:
    
    Formula(std::string InInputFormula, VariablesList Vars);

    void SetVariables(VariablesList Vars);

    std::complex<float> EvaluateOnFly();


protected:
    
    std::string InputFormula;
    
    Parser FormulaParser;
    
    std::shared_ptr<ExpressionBase> FormulaExpression;
    
    bool bHasDirtyFormula;
};
