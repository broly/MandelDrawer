#pragma once
#include <string>

#include "Parser.h"
#include "../Types.h"
#include <iostream>

namespace Mandel
{
    class Formula
    {
    public:
        Formula();
    
        Formula(std::string InInputFormula, std::shared_ptr<VariablesList> Vars = nullptr);


        void SetFormula(std::string InInputFormula);

        void SetVariables(std::shared_ptr<VariablesList> Vars);

        void SetVariable(std::string VarName, Complex* Var);

        void Parse();
    
        Complex EvaluateOnFly();

        bool GetError(std::string& Reason);


    protected:
    
        std::string InputFormula;
    
        Parser FormulaParser;
    
        std::shared_ptr<ExpressionBase> FormulaExpression;
    
        bool bHasDirtyFormula;

        bool bHasError;

        std::string ErrorReason;
    };
}
