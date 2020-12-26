#pragma once
#include <memory>
#include <string>
#include <vector>

#include "Common.h"
#include "Expression.h"

class Parser
{
public:
    Parser()
        : Input(nullptr)
    {}
    
    Parser(const char* InInput, VariablesList InOptionalVars = {})
        : Input(InInput)
        , OptionalVars(InOptionalVars)
    {}
    
    std::string ParseToken();

    void SetVariables(VariablesList InOptionalVars);
    void SetInput(const char* InInput);

    std::shared_ptr<ExpressionBase> ParseSimple(); // Парсит простое выражение
    std::shared_ptr<ExpressionBase> ParseBinary(int min_priority); // Парсит бинарное выражение
    std::shared_ptr<ExpressionBase> Parse();

    bool IsVariable(std::string Token);
    
    const char* Input;
    VariablesList OptionalVars; 
};