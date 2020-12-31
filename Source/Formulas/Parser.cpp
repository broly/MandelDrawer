#include "Parser.h"

#include <stdexcept>



#include "Operators.h"
#include "Functions.h"
#include "../Types.h"

using namespace Mandel;

std::string Parser::ParseToken()
{
    // Pass the spaces       
    while (std::isspace(*Input))
        ++Input;
    
    // Parse this only if starts with digit!
    if (std::isdigit(*Input)) {
        std::string number;
        
        while (std::isdigit(*Input) || *Input == '.' || *Input == 'i')
            number.push_back(*Input++);
        
        return number;
    }

    
    static std::vector<std::string> SpecialTokens =  { "(", ")" };
    
    for (auto& TokenInfo : BinaryOperators)
    {
        if (std::strncmp(Input, TokenInfo.Token.c_str(), TokenInfo.Token.size()) == 0)
        {
            Input += TokenInfo.Token.size();
            return TokenInfo.Token;
        }
    }

    for (auto& FuncInfo : SingleFunctions)
    {
        if (std::strncmp(Input, FuncInfo.FunctionName.c_str(), FuncInfo.FunctionName.size()) == 0)
        {
            Input += FuncInfo.FunctionName.size();
            return FuncInfo.FunctionName;
        }
    }
    
    for (auto& Token : SpecialTokens)
    {
        if (std::strncmp(Input, Token.c_str(), Token.size()) == 0)
        {
            Input += Token.size();
            return Token;
        }
    }

    if (OptionalVars)
        for (auto& pair : OptionalVars->Vars)
        {
            if (std::strncmp(Input, pair.first.c_str(), pair.first.size()) == 0)
            {
                Input += pair.first.size();
                return pair.first;
            }
        }

    if (Input[0] == 0)
        return "";

    const std::string msg = std::string("Unknown token at") + Input;
    
    throw std::runtime_error(msg);
}

void Parser::SetVariables(std::shared_ptr<VariablesList> InOptionalVars)
{
    OptionalVars = InOptionalVars;
}

void Parser::SetVariable(std::string VarName, Complex* Var)
{
    if (!OptionalVars)
        OptionalVars = std::make_shared<VariablesList>();

    OptionalVars->Vars.emplace(VarName, Var);

}

void Parser::SetInput(std::string InInput)
{
    Source = InInput;
    Input = Source.c_str();
}

std::shared_ptr<ExpressionBase> Parser::ParseSimple()
{
    // Try to parse first
    auto token = ParseToken();
    if (token.empty()) 
        throw std::runtime_error("Invalid input");

    if (std::isdigit(token[0])) // Number is expression also (starts with digit)
        return std::make_shared<NumberExpression>(token);

    if (IsVariable(token))
        return std::make_shared<VariableExpression>(token, OptionalVars);

    if (token == "(")
    {
        auto result = Parse();
        auto NextToken = ParseToken();
        if (NextToken != ")")
            throw std::runtime_error("Expected ')'");
        return result; // parse what between braces
    }

    // unary operations handling
    auto arg = ParseSimple(); // arg of operation
    return std::make_shared<UnaryExpression>(token, arg);
}
std::shared_ptr<ExpressionBase> Parser::ParseBinary(int min_priority)
{
    auto left_expr = ParseSimple(); 

    while (true)
    {
        auto op = ParseToken(); // Try to parse binary operation
        const int priority = get_priority(op);
        // go out from loop if priority too low (not binary operation)
        if (priority <= min_priority) {
            Input -= op.size(); // Give token back
            return left_expr; // left expr return
        }

        // right side of binary expression
        auto right_expr = ParseBinary(priority);
        left_expr = std::make_shared<BinaryExpression>(op, left_expr, right_expr); // Update left expr
    } 
}

std::shared_ptr<ExpressionBase> Parser::Parse()
{
    return ParseBinary(0);
}

bool Parser::IsVariable(std::string Token)
{
    if (OptionalVars)
        for (auto& pair : OptionalVars->Vars)
            if (pair.first == Token)
                return true;
    
    return false;
}
