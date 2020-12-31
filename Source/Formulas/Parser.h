#pragma once
#include <memory>
#include <string>
#include <vector>

#include "Common.h"
#include "Expression.h"

namespace Mandel
{
    class Parser
    {
    public:
        Parser()
            : Input(nullptr)
        {}
    
        Parser(const char* InInput, std::shared_ptr<VariablesList> InOptionalVars = nullptr)
            : Input(InInput)
            , OptionalVars(InOptionalVars)
        {}
    
        std::string ParseToken();

        void SetVariables(std::shared_ptr<VariablesList> InOptionalVars);
    
        void SetVariable(std::string VarName, Complex* Var);
    
        void SetInput(std::string InInput);

        std::shared_ptr<ExpressionBase> ParseSimple(); // Simple expression
        std::shared_ptr<ExpressionBase> ParseBinary(int min_priority); // Binary expression
        std::shared_ptr<ExpressionBase> Parse();

        bool IsVariable(std::string Token);
    
        std::string Source;
        const char* Input;
        std::shared_ptr<VariablesList> OptionalVars; 
    };
}