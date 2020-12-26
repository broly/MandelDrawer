#include "Expression.h"

#include <string>


#include "Common.h"
#include "Functions.h"
#include "Operators.h"
#include "CommonTools.h"

bool ReplaceStr(std::string& str, const std::string& from, const std::string& to)
{
    size_t start_pos = str.find(from);
    if (start_pos == std::string::npos)
        return false;
    str.replace(start_pos, from.length(), to);
    return true;
}

std::complex<float> UnaryExpression::Evaluate()
{
    if (const auto func_info = FindBy(SingleFunctions, [=] (const SingleFunctionInfo& v) { return v.FunctionName == Token; }))
    {
        auto v = Argument->Evaluate();
        return func_info->Func(v);
    }
    return {};
}

std::complex<float> BinaryExpression::Evaluate()
{
    if (const auto func_info = FindBy(BinaryOperators, [=] (const BinaryOperator& v) { return Token == v.Token; }))
    {
        auto A = Left->Evaluate();
        auto B = Right->Evaluate();
        return func_info->Func(A, B);
    }
    
    return {};
}

std::complex<float> NumberExpression::Evaluate()
{
    std::string tk = Token;
    const bool bIsImaginary = ReplaceStr(tk, "i", "");
    const float Value = std::stof(tk);
    return bIsImaginary ? std::complex<float>{0.f, Value} : std::complex<float>{Value, 0.f};
}

std::complex<float> VariableExpression::Evaluate()
{
    
    for (auto& pair : Vars.Vars)
    {
        if (pair.first == Token)
            return *pair.second;
    }

    throw std::runtime_error(std::string("Can't find variable: ") + Token);
}
