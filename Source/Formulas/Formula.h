#pragma once
#include <string>

#include "Parser.h"
#include "../Types.h"
#include <iostream>

#include "FormulaInterpreter.h"

namespace Mandel
{
    class Formula
    {
    public:
        Formula();
    
        Formula(std::string InInputFormula, std::shared_ptr<VariablesList> Vars = nullptr);

        ~Formula();


        void SetFormula(std::string InInputFormula);

        void SetVariables(std::shared_ptr<VariablesList> Vars);

        void SetVariable(std::string VarName, Complex* Var);

        void Compile();

        void Parse();
    
        Complex EvaluateOnFly();

        Complex EvaluateCompiled();

        bool GetError(std::string& Reason);


    protected:
    
        std::string InputFormula;
    
        Parser FormulaParser;
    
        std::shared_ptr<ExpressionBase> FormulaExpression;
    
        bool bHasDirtyFormula;

        bool bHasError;

        std::string ErrorReason;

        FormulaInterpreter Interpreter;
    };
}
